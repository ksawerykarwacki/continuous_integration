//
// Created by ksawery.karwacki on 6/6/2021.
//

#ifndef CONTINUOUS_INTEGRATION_REPOSITORY_H
#define CONTINUOUS_INTEGRATION_REPOSITORY_H


#include <string>
#include "Task.h"
#include "Dispatcher.h"

using namespace std;

class Repository {
public:
    void webhook();
    Repository(string name, const bool &shouldBreak, Dispatcher &dispatcher);
    string getStateDescription() const;
    const string &getName() const;
private:
    string name;
    int state;
    Task *task;
    Dispatcher& dispatcher;
    const bool& shouldBreak;
    double randomNumberFromRange(double start, double end);
    double generateCpuUsage();
    double generateRamUsage();
    double generateDuration(double min, double max);
    void runTask(double min, double max, bool getAgent = false, bool getEnv = false);
};

enum States {
    idle = 0,
    waitingForAgent = 1,
    building = 2,
    waitingForDeploy = 3,
    deploying = 4
};

#endif //CONTINUOUS_INTEGRATION_REPOSITORY_H
