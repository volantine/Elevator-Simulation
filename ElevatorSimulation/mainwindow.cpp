#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controlSystem(new ControlSystem(2, 3, this)) {
    ui->setupUi(this);

    ui->label_11->setText("(Elevator buttons can only be called when\n a passenger is in it.)");

    // BUTTON CONNECTING
    connect(ui->StartButton, &QPushButton::released, this, &MainWindow::StartSimulation); // START BUTTON
    connect(ui->StopButton, &QPushButton::released, this, &MainWindow::StopSimulation); // STOP BUTTON
    connect(ui->PauseButton, &QPushButton::released, this, &MainWindow::PauseSimulation); // PAUSE BUTTON
    connect(ui->ContinueButton, &QPushButton::released, this, &MainWindow::ContinueSimulation); // CONTINUE BUTTON
    connect(ui->ClearLog, &QPushButton::released, this, &MainWindow::ClearLogConsole);
    connect(ui->ResetValues, &QPushButton::released, this, &MainWindow::On_ResetButton_Clicked);

    connect(ui->E1Help, &QPushButton::released, this, &MainWindow::HelpHandle);
    connect(ui->E2Help, &QPushButton::released, this, &MainWindow::HelpHandle);
    connect(ui->E1Obstacle, &QPushButton::released, this, &MainWindow::BlockageHandle);
    connect(ui->E2Obstacle, &QPushButton::released, this, &MainWindow::BlockageHandle);
    connect(ui->E1Overload, &QPushButton::released, this, &MainWindow::OverloadingHandle);
    connect(ui->E2Overload, &QPushButton::released, this, &MainWindow::OverloadingHandle);
    connect(ui->E1Open, &QPushButton::released, this, &MainWindow::OpenButtonHandle);
    connect(ui->E2Open, &QPushButton::released, this, &MainWindow::OpenButtonHandle);
    connect(ui->E1Close, &QPushButton::released, this, &MainWindow::CloseButtonHandle);
    connect(ui->E2Close, &QPushButton::released, this, &MainWindow::CloseButtonHandle);
    connect(ui->Fire, &QPushButton::released, this, &MainWindow::FireHandle);
    connect(ui->PowerOut, &QPushButton::released, this, &MainWindow::PowerOutHandle);


    // Set the range for each QSpinBox
    ui->P1StartFloor->setRange(1, 10);
    ui->P1DestFloor->setRange(1, 10);
    ui->P1Time->setRange(1, 99);

    ui->P2StartFloor->setRange(1, 10);
    ui->P2DestFloor->setRange(1, 10);
    ui->P2Time->setRange(1, 99);

    ui->P3StartFloor->setRange(1, 10);
    ui->P3DestFloor->setRange(1, 10);
    ui->P3Time->setRange(1, 99);

    // SPINNERS

    connect(ui->P1StartFloor, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::UpdateSimulationParameters);
    connect(ui->P1DestFloor, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::UpdateSimulationParameters);
    connect(ui->P1Time, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::UpdateSimulationParameters);

    connect(ui->P2StartFloor, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::UpdateSimulationParameters);
    connect(ui->P2DestFloor, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::UpdateSimulationParameters);
    connect(ui->P2Time, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::UpdateSimulationParameters);


    connect(ui->P3StartFloor, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::UpdateSimulationParameters);
    connect(ui->P3DestFloor, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::UpdateSimulationParameters);
    connect(ui->P3Time, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::UpdateSimulationParameters);

    connect(controlSystem, &ControlSystem::updateLog, ui->Log, &QTextEdit::append);
    connect(controlSystem, &ControlSystem::updateTime, this, &MainWindow::UpdateTime);
    connect(controlSystem, &ControlSystem::updateElevator, this, &MainWindow::UpdateElevator);
    connect(controlSystem, &ControlSystem::updatePassenger, this, &MainWindow::UpdatePassenger);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::OpenButtonHandle() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return; // Safety check

    if (button == ui->E1Open) {
        controlSystem->openElevatorDoor(0); // Open door for Elevator 1
    } else if (button == ui->E2Open) {
        controlSystem->openElevatorDoor(1); // Open door for Elevator 2
    }
}


void MainWindow::CloseButtonHandle() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return; // Safety check

    if (button == ui->E1Close) {
        controlSystem->closeElevatorDoor(0); // Close door for Elevator 1
    } else if (button == ui->E2Close) {
        controlSystem->closeElevatorDoor(1); // Close door for Elevator 2
    }
}


void MainWindow::On_ResetButton_Clicked() {
    ui->P1StartFloor->setValue(1);
    ui->P1DestFloor->setValue(1);
    ui->P1Time->setValue(1);

    ui->P2StartFloor->setValue(1);
    ui->P2DestFloor->setValue(1);
    ui->P2Time->setValue(1);

    ui->P3StartFloor->setValue(1);
    ui->P3DestFloor->setValue(1);
    ui->P3Time->setValue(1);
}

/*
-----------------------------------
 Safety Events
-----------------------------------
*/

// Handle help button
void MainWindow::HelpHandle() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return; // Safety check

    if (button == ui->E1Help) {
        controlSystem->Help(0);
    } else if (button == ui->E2Help) {
        controlSystem->Help(1);
    }
}

// Handle blockage button
void MainWindow::BlockageHandle() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return; // Safety check

    if (button == ui->E1Obstacle) {
        controlSystem->ObstacleAlarm(0);
    } else if (button == ui->E2Obstacle) {
        controlSystem->ObstacleAlarm(1);
    }
}

// Handle fire button
void MainWindow::FireHandle() {
    controlSystem->triggerFireAlarm();
}

// Handle power out button
void MainWindow::PowerOutHandle() {
    controlSystem->triggerPowerAlarm();
}

// Handle overloading button
void MainWindow::OverloadingHandle() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return; // Safety check

    if (button == ui->E1Overload) {
        controlSystem->OverloadingAlarm(0);
    } else if (button == ui->E2Overload) {
        controlSystem->OverloadingAlarm(1);
    }
}

/*
-----------------------------------
 INITIALIZATION
-----------------------------------
*/

void MainWindow::StartSimulation() {
    controlSystem->start();
}

void MainWindow::StopSimulation() {
    // Handle stop simulation
    controlSystem->stop();
}

void MainWindow::PauseSimulation() {
    // Handle pause simulation
    controlSystem->pause();
}


void MainWindow::ContinueSimulation() {
    // Handle continue simulation
    controlSystem->continueSim();
}

void MainWindow::ClearLogConsole(){
    ui->Log->clear();
}

void MainWindow::UpdateSimulationParameters() {
    QObject *senderObj = sender(); // Get the sender object (which spin box was changed)
    if (!senderObj) return;

    int passengerIndex = -1;

    // Identify which passenger's input changed
    if (senderObj == ui->P1StartFloor || senderObj == ui->P1DestFloor || senderObj == ui->P1Time) {
        passengerIndex = 0;
    } else if (senderObj == ui->P2StartFloor || senderObj == ui->P2DestFloor || senderObj == ui->P2Time) {
        passengerIndex = 1;
    } else if (senderObj == ui->P3StartFloor || senderObj == ui->P3DestFloor || senderObj == ui->P3Time) {
        passengerIndex = 2;
    }

    if (passengerIndex == -1) return; // No valid passenger identified

    // Get the updated values for the specific passenger
    int startFloor = (passengerIndex == 0) ? ui->P1StartFloor->value() :
                     (passengerIndex == 1) ? ui->P2StartFloor->value() : ui->P3StartFloor->value();

    int destFloor = (passengerIndex == 0) ? ui->P1DestFloor->value() :
                    (passengerIndex == 1) ? ui->P2DestFloor->value() : ui->P3DestFloor->value();

    int timeStep = (passengerIndex == 0) ? ui->P1Time->value() :
                   (passengerIndex == 1) ? ui->P2Time->value() : ui->P3Time->value();

    // Update only the specific passenger
    controlSystem->setSinglePassenger(passengerIndex, startFloor, destFloor, timeStep);
}


void MainWindow::UpdateTime(int time) {
    ui->TimeLabel->setText(QString("Time: %1").arg(time));
}


void MainWindow::UpdateElevator(int id, int floor, Elevator::State state) {
    QString stateText;
    switch (state) {
        case Elevator::Idle: stateText = "Idle"; break;
        case Elevator::MovingUp: stateText = "Moving Up"; break;
        case Elevator::MovingDown: stateText = "Moving Down"; break;
        case Elevator::DoorOpen: stateText = "Door Opens for Passenger"; break;
        case Elevator::Blocked: stateText = "Emergency Mode"; break;
        case Elevator::DoorClosed: stateText = "Door Closes"; break;
    }

    ui->Log->append(QString("\n\nElevator %1 at floor %2, State: %3").arg(id+1).arg(floor).arg(stateText));
}


void MainWindow::UpdatePassenger(int id, int floor, Passenger::State state) {
    QString stateText;
    switch (state) {
        case Passenger::NotRequested: stateText = "is idle"; break;
        case Passenger::Requested: stateText = "requesting an elevator"; break;
        case Passenger::InElevator: stateText = "in an elevator"; break;
        case Passenger::Arrived: stateText = "Arrived"; break;
        case Passenger::Boarding: stateText = "Boarding elevator"; break;
    }

    ui->Log->append(QString("\n\nPassenger %1 is %2 at floor %3.\nPressed button illuminates.").arg(id+1).arg(stateText).arg(floor));
}

