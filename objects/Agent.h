//
// Created by ksawery.karwacki on 6/6/2021.
//

#ifndef CONTINUOUS_INTEGRATION_AGENT_H
#define CONTINUOUS_INTEGRATION_AGENT_H

#include <string>
#include <vector>
#include "Task.h"

using namespace std;

class Agent {
private:
    string name;
    double cpu;
    double ram;
    vector<Task*> tasks;
public:
    bool canRunTask(Task *task);

    const vector<Task *> &getTasks() const;

    const string &getName() const;

    Agent(const string &name, double cpu, double ram);

    void addTask(Task *task);
    void removeTask(Task *task);
    string getResourcesStatus();
};


#endif //CONTINUOUS_INTEGRATION_AGENT_H
