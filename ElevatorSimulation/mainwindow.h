#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QApplication>
#include <QTextEdit>
#include <QWidget>
#include "ControlSystem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Output Log Function
    void AddToLog(std::string);

private:
    Ui::MainWindow *ui;
    ControlSystem *controlSystem;

 private slots:
    void UpdateSimulationParameters();
    void UpdatePassenger(int id, int floor, Passenger::State state);

    void UpdateTime(int time);
    void UpdateElevator(int id, int floor, Elevator::State state);
    void On_ResetButton_Clicked();

    void StartSimulation();
    void StopSimulation();
    void ContinueSimulation();
    void PauseSimulation();
    void ClearLogConsole();

    void OpenButtonHandle();
    void CloseButtonHandle();

    void PowerOutHandle();
    void FireHandle();
    void HelpHandle();
    void BlockageHandle();
    void OverloadingHandle();
};
#endif // MAINWINDOW_H
