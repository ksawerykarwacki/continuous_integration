//
// Created by ksawery.karwacki on 6/6/2021.
//

#include "Dispatcher.h"

void Dispatcher::getAgent(Task *task) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [task, this] {
        bool foundAgent = false;
        for(auto &agent : agents) {
            foundAgent = agent.canRunTask(task);
            if(foundAgent) {
                agent.addTask(task);
                task->agent = &agent;
                break;
            }
        }
        return foundAgent;
    });
    tasks.push_back(task);
}

Dispatcher::Dispatcher(vector<Task *> &tasks, vector<Agent> &agents, Environment &environment, mutex &mtx,
                       mutex &envMtx)
        : tasks(tasks), agents(agents), environment(environment), mtx(mtx), envMtx(envMtx) {}

void Dispatcher::releaseAgent(Task *task) {
    std::unique_lock<std::mutex> lock(mtx);
    task->agent->removeTask(task);
    lock.unlock();
    cv.notify_all();
}

void Dispatcher::getEnvironment(Task *task) {
    std::unique_lock<std::mutex> lock(envMtx);
    envCv.wait(lock, [this] {
        return environment.getTask() == nullptr;
    });
    environment.setTask(task);
}

void Dispatcher::releaseEnvironment() {
    std::unique_lock<std::mutex> lock(mtx);
    environment.setTask(nullptr);
    lock.unlock();
    envCv.notify_one();
}
