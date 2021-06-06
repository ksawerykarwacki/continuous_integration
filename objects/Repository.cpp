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
        this->state = idle;
        this->wait(3, 5);
        this->state = building;
        this->wait(1.5, 4);
    }
}

void Repository::wait(double min, double max) {
    stateCompleteness = 0;
    double value = randomNumberFromRange(min, max) * 1000000;
    auto steps = value / 400000 +1 ;
    auto stepDuration = value / steps;
    for(int i = 0; i < steps; i++)
    {
        if(shouldBreak)
        {
            break;
        }
        usleep(stepDuration);
        stateCompleteness = 1.0 * (i + 1) / steps * 100;
    }
}

Repository::Repository(string name, const bool &shouldBreak) : name(std::move(name)), shouldBreak(shouldBreak) {
    state = idle;
}

std::string Repository::getStateDescription() {
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
