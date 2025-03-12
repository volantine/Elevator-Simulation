#include "mainwindow.h"
#include <QApplication>

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <algorithm>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    std::cout << "[NOTICE] Setting up simulation." << std::endl;
    return a.exec();
}
