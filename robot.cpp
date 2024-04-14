#include "robot.h"

Robot::Robot() {
    mQuery.exec("DROP TABLE IF EXISTS robots");
    mQuery.exec("CREATE TABLE robots ("
                "robot_id INT PRIMARY KEY AUTO_INCREMENT,"
                "name CHAR(125),"
                "current_task CHAR(125))");
}

Robot::~Robot() {
    mQuery.exec("DROP TABLE IF EXISTS robots");
}

void Robot::printRobots() {
    std::cout << "Robots:" << std::endl;
    if (!mQuery.exec("SELECT * FROM robots")) {
        std::cout << "Error: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    while (mQuery.next()) {
        int id = mQuery.value(0).toInt();
        QString name = mQuery.value(1).toString();
        QString task = mQuery.value(2).toString();

        QString idString = QString("Robot ID: %1").arg(id).leftJustified(15, ' ');
        QString nameString = QString("Name: %1").arg(name).leftJustified(20, ' ');
        QString taskString = QString("Current Task: %1").arg(task).leftJustified(15, ' ');

        qDebug() << qPrintable(idString)
                 << qPrintable(nameString)
                 << qPrintable(taskString);
    }
}

void Robot::addRobot() {
    std::string name;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter robot name: ";
    std::getline(std::cin, name);

    mQuery.prepare("INSERT INTO robots (name) "
                   "VALUES (:name)");
    mQuery.bindValue(":name", QString::fromStdString(name));
    if (!mQuery.exec()) {
        std::cout << "Error adding robot: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }
    std::cout << "Robot added successfully" << std::endl;
}

void Robot::deleteRobot() {
    int robot_id;

    std::cout << "Enter robot id: ";
    if (!(std::cin >> robot_id)) {
        std::cout << "Invalid input for robot id" << std::endl;
        return;
    }

    mQuery.prepare("DELETE FROM robots WHERE robot_id = :robot_id");
    mQuery.bindValue(":robot_id", robot_id);
    if (!mQuery.exec()) {
        std::cout << "Error deleting robot: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }
    std::cout << "Robot deleted successfully" << std::endl;
}

void Robot::assignTask() {
    int robot_id;
    std::string task;

    std::cout << "Enter robot id: ";
    std::cin >> robot_id;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    mQuery.prepare("SELECT COUNT(*) FROM robots WHERE robot_id = :robot_id");
    mQuery.bindValue(":robot_id", robot_id);
    if (!mQuery.exec()) {
        std::cout << "Error executing query: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }
    if (mQuery.next() && mQuery.value(0).toInt() == 0) {
        std::cout << "Robot with ID " << robot_id << " does not exist" << std::endl;
        return;
    }

    mQuery.prepare("SELECT current_task FROM robots WHERE robot_id = :robot_id");
    mQuery.bindValue(":robot_id", robot_id);
    if (!mQuery.exec()) {
        std::cout << "Error executing query: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }
    if (mQuery.next() && !mQuery.value(0).isNull()) {
        std::cout << "Robot with ID " << robot_id << " already has a task assigned" << std::endl;
        return;
    }

    std::cout << "Enter task name: ";
    std::getline(std::cin, task);

    mQuery.prepare("SELECT description FROM task WHERE description = :description");
    mQuery.bindValue(":description", QString::fromStdString(task));
    if (!mQuery.exec()) {
        std::cout << "Error executing query: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    if (mQuery.next()) {
        QString description = mQuery.value(0).toString();

        assignmentQuery.prepare("SELECT robot_id FROM robots WHERE current_task = :description");
        assignmentQuery.bindValue(":description", description);
        if (!assignmentQuery.exec()) {
            std::cout << "Error executing query: " << assignmentQuery.lastError().text().toStdString() << std::endl;
            return;
        }
        if (assignmentQuery.next()) {
            std::cout << "Task is already assigned to a robot" << std::endl;
            return;
        }

        updateQuery.prepare("UPDATE robots SET current_task = :task_description WHERE robot_id = :robot_id");
        updateQuery.bindValue(":task_description", QString::fromStdString(task));
        updateQuery.bindValue(":robot_id", robot_id);
        if (!updateQuery.exec()) {
            std::cout << "Error assigning task: " << updateQuery.lastError().text().toStdString() << std::endl;
            return;
        }
        std::cout << "Task assigned successfully" << std::endl;
    } else {
        std::cout << "Task not found" << std::endl;
    }
}

void Robot::run() {
    mQuery.prepare("SELECT robot_id, current_task FROM robots WHERE current_task IS NOT NULL");
    if (!mQuery.exec()) {
        std::cout << "Error executing query: " << mQuery.lastError().text().toStdString() << std::endl;
        return;
    }

    while (mQuery.next()) {
        int robot_id = mQuery.value(0).toInt();
        QString task_description = mQuery.value(1).toString();

        durationQuery.prepare("SELECT time FROM task WHERE description = :description");
        durationQuery.bindValue(":description", task_description);
        if (!durationQuery.exec()) {
            std::cout << "Error retrieving task duration: " << durationQuery.lastError().text().toStdString() << std::endl;
            return;
        }
        if (!durationQuery.next()) {
            std::cout << "Task duration not found" << std::endl;
            return;
        }
        int task_duration = durationQuery.value(0).toInt();

        std::cout << "Robot ID: " << robot_id << ", Task: " << task_description.toStdString() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(task_duration));
        std::cout << "Task completed" << std::endl;

        updateQuery.prepare("DELETE FROM task WHERE description = :description");
        updateQuery.bindValue(":description", task_description);
        if (!updateQuery.exec()) {
            std::cout << "Error deleting task: " << updateQuery.lastError().text().toStdString() << std::endl;
            return;
        }
        updateQuery.prepare("UPDATE robots SET current_task = NULL WHERE robot_id = :robot_id");
        updateQuery.bindValue(":robot_id", robot_id);
        if (!updateQuery.exec()) {
            std::cout << "Error updating task: " << updateQuery.lastError().text().toStdString() << std::endl;
            return;
        }
        std::cout << std::endl;
    }
}

