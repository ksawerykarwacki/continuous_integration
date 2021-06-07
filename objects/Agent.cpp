//
// Created by ksawery.karwacki on 6/6/2021.
//

#include "Agent.h"

bool Agent::canRunTask(Task *task) {
    double freeCpu = cpu;
    double freeRam = ram;
    for(auto t : tasks) {
        freeCpu -= t->cpu;
        freeRam -= t->ram;
    }
    return freeCpu >=task->cpu && freeRam >=task->ram;
}

void Agent::addTask(Task *task) {
    tasks.push_back(task);
}

void Agent::removeTask(Task *task) {
    auto current_task = tasks.begin();

    while(current_task < tasks.end()) {
        if(*current_task == task) {
            current_task = tasks.erase(current_task);
        } else {
            ++current_task;
        }
    }
}

Agent::Agent(const string &name, double cpu, double ram) : name(name), cpu(cpu), ram(ram) {}

const string &Agent::getName() const {
    return name;
}

const vector<Task *> &Agent::getTasks() const {
    return tasks;
}

string Agent::getResourcesStatus() {
    double usedCpu = 0;
    double usedRam = 0;
    for(auto task : tasks) {
        usedCpu += task->cpu;
        usedRam += task->ram;
    }
    return "CPU: " + to_string(usedCpu) + "/" + to_string(cpu) + " MEM: " + to_string(usedRam) + "/" + to_string(ram);
}
