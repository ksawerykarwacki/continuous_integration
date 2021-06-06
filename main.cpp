#include <curses.h>
#include <thread>
#include <vector>
#include <ctime>
#include <unistd.h>

using namespace std;

bool shouldBreak = false;
vector<thread> threads;

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
    }
}

void writeState() {
    while(!shouldBreak) {
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        clear();
        move(0,0);
        attron(COLOR_PAIR(1));
        printw("❤ ę Hello %s",std::ctime(&end_time));
        refresh();

        for(int i = 0; i < 500; i++) {
            if(shouldBreak) {
                break;
            }
            usleep(1000);
        }
    }
}

int main() {
    setlocale(LC_ALL, "");
    initscr();
    timeout(-1);
    noecho();
    raw();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, COLOR_WHITE);

    threads.emplace_back(&listenForKey);
    threads.emplace_back(&writeState);

    for(int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }

    endwin();
    return 0;
}
