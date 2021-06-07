#include <cursesw.h>
#include <thread>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <clocale>
#include <atomic>
#include "objects/Repository.h"

using namespace std;

bool shouldBreak = false;
Environment* environment;
vector<Repository> repositories;
vector<Task*> tasks;
vector<Agent> agents;
enum Views {
    repoView = 0,
    taskView = 1,
    agentView = 2
};

atomic<int> view = repoView;
atomic<unsigned long> offset = 0;
unsigned long itemsPerPage = 20;

int randomNumberFromRange(double start, double end) {
    double result = (double)rand() / RAND_MAX;
    return (int)(start + result * (end - start));
}

unsigned long getSize() {
    switch(view) {
        case repoView:
            return repositories.size();
        case taskView:
            return tasks.size();
        case agentView:
            return agents.size();
        default:
            return 0;
    }
}

void listenForKey()
{
    while(true)
    {
        int c = getch();
        if(c == 27 || c == 113)
        {
            shouldBreak = true;
            break;
        }
        if(c == 49) {
            view = repoView;
        } else if(c == 50) {
            view = taskView;
        } else if(c == 51) {
            view = agentView;
        }
        if(c == KEY_DOWN) {
            if(getSize() > offset + itemsPerPage) {
                offset++;
            }
        }
        if(c == KEY_UP) {
            if(offset > 0) {
                offset--;
            }
        }
    }
}

void writeState() {
    while(!shouldBreak) {
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        erase();
        itemsPerPage = LINES - 5;
        move(0,0);
        attron(COLOR_PAIR(2));
        printw("System working, current time: %s",std::ctime(&end_time));
        if(view == repoView) {
            std::string text = "Repository";
            int repoNameLength = text.size();
            for(const auto& repo : repositories) {
                repoNameLength = max(repoNameLength, (int)repo.getName().size());
            }
            move(2, 0);
            text.append(repoNameLength-text.length()+1, ' ');
            text = text + "Status";
            text.append(COLS-text.length(), ' ');
            attron(COLOR_PAIR(11));
            printw(text.c_str());
            for(int i=0; i < repositories.size(); i++) {
                move(3+i, 0);
                attron(COLOR_PAIR(1));
                printw("%s", repositories[i].getName().c_str());
                move(3+i, repoNameLength + 1);
                int state = repositories[i].getState();
                if( state == building || state == deploying) {
                    attron(COLOR_PAIR(3));
                } else if (state == waitingForDeploy || state == waitingForAgent ) {
                    attron(COLOR_PAIR(4));
                } else {
                    attron(COLOR_PAIR(2));
                }
                printw("%s", repositories[i].getStateDescription().c_str());
            }
        } else if (view == taskView) {
            std::string text = "Task";
            int taskColumnLength = text.size();
            for(auto task : tasks) {
                int length = task->getJobId().size() + task->agent->getName().size()+4;
                taskColumnLength = max(taskColumnLength, length);
            }
            move(2, 0);
            text.append(taskColumnLength-text.length()+1, ' ');
            std::string text2 = "Status";
            text2.append(text2.length()+1, ' ');
            std::string text3 = "Progress";
            text3.append(text3.length()+1, ' ');
            text += text2 + text3;
            text.append(COLS-text.length(), ' ');
            attron(COLOR_PAIR(11));
            printw(text.c_str());
            for(unsigned long i=1; i<=min(itemsPerPage, tasks.size()); i++) {
                Task *task = tasks[tasks.size()-i-offset];
                move(2+i, 0);
                attron(COLOR_PAIR(1));
                printw("%s", task->getJobId().c_str());
                attron(COLOR_PAIR(2));
                printw(" on ");
                attron(COLOR_PAIR(7));
                printw("%s", task->agent->getName().c_str());
                move(2+i, taskColumnLength+1);
                if(task->getStatus() == active) {
                    attron(COLOR_PAIR(8));
                } else if (task->getStatus() == starting) {
                    attron(COLOR_PAIR(10));
                } else {
                    attron(COLOR_PAIR(9));
                }
                printw(" %s ", task->getStatusDescription().c_str());
                if(task->getStatus() == active) {
                    move(2+i, taskColumnLength+13);
                    attron(COLOR_PAIR(2));
                    printw(" %s of %s", task->getTime(task->getElapsed()).c_str(), task->getTime(task->duration).c_str());
                }
            }
            move(LINES-2, 0);
            attron(COLOR_PAIR(2));
            printw("Items %d-%d from %d", min(offset+1, getSize()), min(itemsPerPage+offset, getSize()), getSize());
        } else if (view == agentView) {
            int linesOffset = 0;
            move(2, 0);
            attron(COLOR_PAIR(2));
            printw("Environment ");
            attron(COLOR_PAIR(5));
            printw("%s", environment->getName().c_str());
            attron(COLOR_PAIR(2));
            printw(": ");
            if(environment->getTask() == nullptr) {
                attron(COLOR_PAIR(3));
                printw("IDLE");
            } else {
                attron(COLOR_PAIR(2));
                printw("deploying ");
                attron(COLOR_PAIR(1));
                printw("%s", environment->getTask()->getJobId().c_str());
            }
            for(int i=0; i < agents.size(); i++) {
                move(4+i+linesOffset, 0);
                attron(COLOR_PAIR(7));
                printw("%s", agents[i].getName().c_str());
                attron(COLOR_PAIR(2));
                printw("(%d)", agents[i].getTasks().size());
                printw(" | %s", agents[i].getResourcesStatus().c_str());
                for(auto task : agents[i].getTasks()) {
                    linesOffset++;
                    move(4+i+linesOffset, 0);
                    attron(COLOR_PAIR(2));
                    printw(" - ");
                    attron(COLOR_PAIR(1));
                    printw("%s", task->getJobId().c_str());
                    attron(COLOR_PAIR(2));
                    printw(" | %s", task->getResourcesQuota().c_str());
                }
            }
        }

        move(LINES-1, 0);
        vector<std::string> texts;
        int sizeOfText = 0;

        texts.push_back(" Repositories[1] ");
        texts.push_back(" Tasks[2] ");
        texts.push_back(" Agents[3] ");
        texts.push_back(" Quit[q] ");
        for(int i=0; i<texts.size(); i++) {
            if(i==view) {
                attron(COLOR_PAIR(11));
            } else {
                attron(COLOR_PAIR(6));
            }
            printw("%s", texts[i].c_str());
            sizeOfText += texts[i].size();
            if(i<texts.size()-1) {
                attron(COLOR_PAIR(6));
                printw("|");
                sizeOfText++;
            }
        }
        std::string text = "";
        text.append(COLS-sizeOfText, ' ');
        attron(COLOR_PAIR(6));
        printw(text.c_str());
        refresh();

        usleep(1000);
    }
}

int main() {
    setlocale(LC_ALL, "");
    initscr();
    timeout(-1);
    noecho();
    raw();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, COLOR_WHITE);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_YELLOW);
    init_pair(9, COLOR_WHITE, COLOR_GREEN);
    init_pair(10, COLOR_WHITE, COLOR_BLUE);
    init_pair(11, COLOR_BLACK, COLOR_GREEN);

    environment = new Environment("e2e");
    Dispatcher dispatcher = Dispatcher(tasks, agents, *environment);

    vector<thread> threads;

    threads.emplace_back(&listenForKey);
    threads.emplace_back(&writeState);

    string taskNames[] = {"web", "shop", "blog", "service-desk", "transactions-management", "warehouse-management", "mobile-app"};
    for(auto name : taskNames) {
        repositories.emplace_back(name, shouldBreak, dispatcher);
    }

    string agentNames[] = {"tortuga", "haiti", "boreas", "sirocco"};
    for(auto name : agentNames) {
        agents.emplace_back(name, randomNumberFromRange(1, 8), randomNumberFromRange(512, 16000));
    }

    for(int i=0; i < repositories.size(); i++) {
        threads.emplace_back(&Repository::webhook, &repositories[i]);
    }

    for(int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }

    endwin();
    return 0;
}
