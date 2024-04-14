#include "client.h"
#include "robot.h"

int main()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("robot_worker"); // Change database name
    db.setUserName("root");             // Change username
    db.setPassword("password");         // Change password
    db.open();
    if (!db.isOpen()) {
        std::cerr << "Error: Unable to open database\n";
        return 1;
    }

    QVariantList tasks, times; 
    tasks << "Get garbage" << "Clean stairs" << "Debug the code";
    times << 10 << 30 << 20;

    Client myClient;
    Robot myRobot;
    myClient.init(tasks, times);

    int choice;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Print tasks\n";
        std::cout << "2. Add task\n";
        std::cout << "3. Print robots\n";
        std::cout << "4. Add robot\n";
        std::cout << "5. Delete robots\n";
        std::cout << "6. Assign task to robot\n";
        std::cout << "7. Run tasks\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cout << std::endl;

        switch (choice) {
            case 1:
                myClient.printTasks();
                break;
            case 2:
                myClient.addTask();
                break;
            case 3:
                myRobot.printRobots();
                break;
            case 4:
                myRobot.addRobot();
                break;
            case 5:
                myRobot.deleteRobot();
                break;
            case 6:
                myRobot.assignTask();
                break;
            case 7:
                myRobot.run();
                break;
            case 0:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}

