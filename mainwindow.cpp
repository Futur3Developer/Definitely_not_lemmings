#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->newGame, &QPushButton::clicked, this, &MainWindow::startNewGame);
    connect(ui->quit, &QPushButton::clicked, this, &MainWindow::quitProgram);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startNewGame()
{
    Game::Get();
    this->close();
}


void MainWindow::quitProgram()
{
    this->close();
}

