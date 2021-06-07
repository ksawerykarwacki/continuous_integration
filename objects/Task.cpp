//
// Created by ksawery.karwacki on 6/6/2021.
//

#include <unistd.h>
#include "Task.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip>

Task::Task(Repository *repository, double cpu, double ram, double duration, const bool &shouldBreak)
        : repository(repository), cpu(cpu),
          ram(ram), duration(duration), shouldBreak(shouldBreak) {
    status = starting;
}

void Task::run() {
    status = active;
    elapsed = 0;
    auto steps = duration / 1000000 +1 ;
    auto stepDuration = duration / steps;
    for(int i = 0; i < steps; i++)
    {
        if(shouldBreak)
        {
            break;
        }
        elapsed = i * stepDuration;
        usleep(stepDuration);
    }
    status = finished;
}

std::string Task::getStatusDescription() const {
    switch(status) {
        case starting:
            return "STARTING";
        case active:
            return "RUNNING";
        case finished:
            return "FINISHED";
        default:
            return "UNKNOWN";
    }
}

double Task::getElapsed() const {
    return elapsed;
}

int Task::getStatus() const {
    return status;
}

std::string Task::getTime(double time) {
    int t = time / 1000000;
    std::stringstream mm;
    mm << std::setw(2) << std::setfill('0') << t/60;
    std::string minutes = mm.str();
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << t % 60;
    std::string seconds = ss.str();

    return minutes+"m:"+seconds+"s";
}

std::string Task::getResourcesQuota() {
    return "CPU: " + std::to_string(cpu) + " MEM: " + std::to_string(ram);
}
