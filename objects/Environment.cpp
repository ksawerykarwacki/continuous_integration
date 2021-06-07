//
// Created by ksawery.karwacki on 6/7/2021.
//

#include "Environment.h"

Task *Environment::getTask() const {
    return task;
}

const std::atomic<bool> &Environment::getDeploying() const {
    return deploying;
}

void Environment::setTask(Task *task) {
    Environment::task = task;
}

void Environment::setDeploying(bool deploying) {
    Environment::deploying = deploying;
}

Environment::Environment(const std::string &name) : name(name) {}
