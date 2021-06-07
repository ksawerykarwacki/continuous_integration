//
// Created by ksawery.karwacki on 6/7/2021.
//

#ifndef CONTINUOUS_INTEGRATION_ENVIRONMENT_H
#define CONTINUOUS_INTEGRATION_ENVIRONMENT_H


#include <atomic>
#include "Task.h"

class Environment {
private:
    Task *task;
    std::atomic<bool> deploying;
    std::string name;
public:
    Environment(const std::string &name);

public:
    void setTask(Task *task);

    void setDeploying(bool deploying);

public:
    Task *getTask() const;

    const std::atomic<bool> &getDeploying() const;
};


#endif //CONTINUOUS_INTEGRATION_ENVIRONMENT_H
