//
// Created by ksawery.karwacki on 6/6/2021.
//

#ifndef CONTINUOUS_INTEGRATION_TASK_H
#define CONTINUOUS_INTEGRATION_TASK_H

#include <string>

class Repository;
class Agent;

class Task {
public:
    Repository *repository;
    Agent *agent;
    double cpu;
    double ram;
    double duration;

    Task(Repository *repository, double cpu, double ram, double duration, const bool &shouldBreak);
    void run();

    int getStatus() const;

    double getElapsed() const;
    std::string getTime(double time);
    std::string getStatusDescription() const;
private:
    const bool& shouldBreak;
    double elapsed;
    int status;
};

enum Status {
    starting = 0,
    active = 1,
    finished = 2
};
#endif //CONTINUOUS_INTEGRATION_TASK_H
