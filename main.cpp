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
unsigned long itemsPerPage = LINES - 4;

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
            offset = max(--offset, 0UL );
        }
    }
}

void writeState() {
    while(!shouldBreak) {
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        erase();
        move(0,0);
        attron(COLOR_PAIR(1));
        printw("System working, current time: %s",std::ctime(&end_time));
        if(view == repoView) {
            for(int i=0; i < repositories.size(); i++) {
                move(2+i, 0);
                printw("%s: ", repositories[i].getName().c_str());
                printw("%s", repositories[i].getStateDescription().c_str());
                if(repositories[i].getTask() != nullptr) {
                    printw(" | %s", repositories[i].getTask()->getResourcesQuota().c_str());
                }
            }
        } else if (view == taskView) {
            move(1, 0);
            for(unsigned long i=1; i<=min(itemsPerPage, tasks.size()); i++) {
                Task *task = tasks[tasks.size()-i-offset];
                move(1+i, 0);
                printw("%s on ", task->repository->getName().c_str());
                printw("%s: ", task->agent->getName().c_str());
                printw("%s", task->getStatusDescription().c_str());
                if(task->getStatus() == active) {
                    printw(" %s of %s", task->getTime(task->getElapsed()).c_str(), task->getTime(task->duration).c_str());
                }
            }
            move(LINES-2, 0);
            printw("Items %d-%d from %d", min(offset+1, getSize()), min(itemsPerPage+offset, getSize()), getSize());
        } else if (view == agentView) {
            for(int i=0; i < agents.size(); i++) {
                move(2+i, 0);
                printw("%s:", agents[i].getName().c_str());
                printw("%d", agents[i].getTasks().size());
                printw(" | %s", agents[i].getResourcesStatus().c_str());
            }
        }

        move(LINES-1, 0);
        std::string text = "  Press q or ESC to exit";
        text.append(COLS-text.length(), ' ');
        attron(COLOR_PAIR(6));
        printw(text.c_str());
        refresh();

//        for(int i = 0; i < 500; i++) {
//            if(shouldBreak) {
//                break;
//            }
//            usleep(1000);
//        }
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
