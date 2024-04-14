#ifndef ROBOT_H
#define ROBOT_H

#include "client.h"

class Robot
{
public:
    Robot();
    ~Robot();

    void printRobots();
    void addRobot();
    void deleteRobot();
    void assignTask();
    void run();
    
private:
    QSqlQuery mQuery, assignmentQuery, updateQuery, durationQuery;
};

#endif // ROBOT_H

