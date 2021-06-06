//
// Created by ksawery.karwacki on 6/6/2021.
//

#ifndef CONTINUOUS_INTEGRATION_REPOSITORY_H
#define CONTINUOUS_INTEGRATION_REPOSITORY_H


#include <string>

using namespace std;

class Repository {
public:
    void webhook();
    Repository(string name, const bool &shouldBreak);
    string getStateDescription();
    const string &getName() const;
private:
    string name;
    int state;
    int stateCompleteness;
    const bool& shouldBreak;
    double randomNumberFromRange(double start, double end);
    void wait();

    void wait(double min, double max);
};

enum States {
    idle = 0,
    waitingForAgent = 1,
    building = 2,
    waitingForDeploy = 3,
    deploying = 4
};

#endif //CONTINUOUS_INTEGRATION_REPOSITORY_H
