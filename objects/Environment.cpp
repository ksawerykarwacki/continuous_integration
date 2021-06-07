//
// Created by ksawery.karwacki on 6/7/2021.
//

#include "Environment.h"

Task *Environment::getTask() const {
    return task;
}

void Environment::setTask(Task *task) {
    Environment::task = task;
}

Environment::Environment(const std::string &name) : name(name) {}
