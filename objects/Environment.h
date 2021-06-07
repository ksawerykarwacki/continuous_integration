//
// Created by ksawery.karwacki on 6/7/2021.
//

#ifndef CONTINUOUS_INTEGRATION_ENVIRONMENT_H
#define CONTINUOUS_INTEGRATION_ENVIRONMENT_H


#include <atomic>
#include "Task.h"

class Environment {
private:
    std::atomic<Task*> task;
    std::string name;
public:
    explicit Environment(const std::string &name);

public:
    void setTask(Task *task);

public:
    Task *getTask() const;
};


#endif //CONTINUOUS_INTEGRATION_ENVIRONMENT_H
