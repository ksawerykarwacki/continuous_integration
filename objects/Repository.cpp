//
// Created by ksawery.karwacki on 6/6/2021.
//

#include <unistd.h>

#include <utility>
#include "Repository.h"

double Repository::randomNumberFromRange(double start, double end) {
    double result = (double)rand() / RAND_MAX;
    return (start + result * (end - start));
}

void Repository::webhook() {
    while(true) {
        if(shouldBreak) {
            break;
        }
        this->runTask(3.5, 5);
        this->runTask(12, 40, true);
        this->runTask(5, 15, true, true);
    }
}

void Repository::runTask(double min, double max, bool getAgent, bool getEnv) {
    this->state = idle;
    this->task = new Task(this, generateCpuUsage(), generateRamUsage(),
                          generateDuration(min, max), shouldBreak);
    if(getAgent) {
        this->state = waitingForAgent;
        this->dispatcher.getAgent(task);
        if(getEnv) {
            this->state = waitingForDeploy;
            this->dispatcher.getEnvironment(task);
            this->state = deploying;
        } else {
            this->state = building;
        }
    }
    this->task->run();

    if(getAgent) {
        this->dispatcher.releaseAgent(task);
        if(getEnv) {
            this->dispatcher.releaseEnvironment();
        }
    }

    this->state = idle;
    this->task = nullptr;
}

Repository::Repository(string name, const bool &shouldBreak, Dispatcher &dispatcher)
        : name(std::move(name)), shouldBreak(shouldBreak), dispatcher(dispatcher) {
    state = idle;
}

std::string Repository::getStateDescription() const {
    switch(this->state) {
        case idle:
            return "IDLE";
        case waitingForAgent:
            return "WAITING FOR AGENT";
        case building:
            return "BUILDING";
        case waitingForDeploy:
            return "WAITING FOR ENV";
        case deploying:
            return "DEPLOYING";
        default:
            return "UNKNOWN";
    }
}

const string &Repository::getName() const {
    return name;
}

double Repository::generateCpuUsage() {
    return randomNumberFromRange(0.5, 4.0);
}

double Repository::generateRamUsage() {
    return randomNumberFromRange(512, 8192);
}

double Repository::generateDuration(double min, double max) {
    return randomNumberFromRange(min, max) * 1000000;
}

Task *Repository::getTask() const {
    return task;
}

int Repository::getState() const {
    return state;
}
