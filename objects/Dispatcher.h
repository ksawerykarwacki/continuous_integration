//
// Created by ksawery.karwacki on 6/6/2021.
//

#ifndef CONTINUOUS_INTEGRATION_DISPATCHER_H
#define CONTINUOUS_INTEGRATION_DISPATCHER_H


#include <vector>
#include <mutex>
#include <condition_variable>
#include "Agent.h"
#include "Environment.h"

class Task;

class Dispatcher {
private:
    std::vector<Task*>& tasks;
    std::vector<Agent>& agents;
    Environment& environment;
public:
    Dispatcher(vector<Task *> &tasks, vector<Agent> &agents, Environment &environment, mutex &mtx,
               mutex &envMtx);

private:
    std::mutex& mtx;
    std::condition_variable cv;
    std::mutex& envMtx;
    std::condition_variable envCv;
public:
    void getAgent(Task *task);
    void releaseAgent(Task *task);
    void getEnvironment(Task *task);
    void releaseEnvironment();
};


#endif //CONTINUOUS_INTEGRATION_DISPATCHER_H
