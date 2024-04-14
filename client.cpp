#include "client.h"

Client::Client() {
    mQuery.exec("DROP TABLE IF EXISTS task");
    mQuery.exec("CREATE TABLE task ("
                "task_id INT PRIMARY KEY AUTO_INCREMENT,"
                "description CHAR(125),"
                "time INT NOT NULL)");
}

Client::~Client() {
    mQuery.exec("DROP TABLE IF EXISTS task");
}

void Client::init(QVariantList tasks, QVariantList times){
    mQuery.prepare("INSERT INTO task (description, time) VALUES (:description, :time)");
    mTasks = tasks;
    mTimes = times;
    if (mTasks.size() != mTimes.size()) {
        qDebug() << "Error: Number of tasks and times do not match";
        return;
    }
    for (int i = 0; i < mTasks.size(); ++i) {
        mQuery.bindValue(":description", mTasks.at(i));
        mQuery.bindValue(":time", mTimes.at(i));
        mQuery.exec();
    }
}

void Client::printTasks() {
    std::cout << "All tasks:" << std::endl;
    if (!mQuery.exec("SELECT * FROM task")) {
        qDebug() << "Error executing query:" << mQuery.lastError().text();
        return;
    }

    while (mQuery.next()) {
        int id = mQuery.value(0).toInt();
        QString description = mQuery.value(1).toString();
        int time = mQuery.value(2).toInt();

        QString idString = QString("Task_id: %1").arg(id).leftJustified(15, ' ');
        QString descriptionString = QString("Description: %1").arg(description).leftJustified(40, ' ');
        QString timeString = QString("Time: %1").arg(time).leftJustified(15, ' ');

        qDebug() << qPrintable(idString)
                 << qPrintable(descriptionString)
                 << qPrintable(timeString);
    }
}

void Client::addTask() {
    std::string task;
    int time;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter task name: "; 
    std::getline(std::cin, task);

    std::cout << "Enter task duration: "; 
    std::cin >> time;

    mQuery.prepare("INSERT INTO task (description, time) "
                  "VALUES (:description, :time)");
    mQuery.bindValue(":description", QString::fromStdString(task));
    mQuery.bindValue(":time", time);
    if (!mQuery.exec()) {
        qDebug() << "Error adding task:" << mQuery.lastError().text();
        return;
    }
    qDebug() << "Task added successfully";
}

