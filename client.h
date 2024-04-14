#ifndef CLIENT_H
#define CLIENT_H

#include <QtSql>
#include <QSqlDatabase>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <thread>

class Client
{
public:
    Client();
    ~Client();

    void init(QVariantList tasks, QVariantList times);
    void printTasks();
    void addTask();

private:
    QSqlQuery mQuery;
    QVariantList mTasks, mTimes;
};  

#endif // CLIENT_H

