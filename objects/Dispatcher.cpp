//
// Created by ksawery.karwacki on 6/6/2021.
//

#include "Dispatcher.h"

void Dispatcher::getAgent(Task *task) {
    std::unique_lock<std::mutex> lock(mtx);
    auto& ag = this->agents;
    cv.wait(lock, [task, ag]{
        bool foundAgent = false;
        for(auto a : ag) {
            if(a.canRunTask(task)) {
                foundAgent = true;
                break;
            }
        }
        return foundAgent;
    });
    for(auto &a : ag) {
        if(a.canRunTask(task)) {
            a.addTask(task);
            task->agent = &a;
            tasks.push_back(task);
            break;
        }
    }
}

Dispatcher::Dispatcher(vector<Task *> &tasks, vector<Agent> &agents, Environment &environment)
        : tasks(tasks), agents(agents), environment(environment) {}

void Dispatcher::releaseAgent(Task *task) {
    std::unique_lock<std::mutex> lock(mtx);
    task->agent->removeTask(task);
    lock.unlock();
    cv.notify_all();
}
