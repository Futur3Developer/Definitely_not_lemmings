#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->new_game, &QPushButton::clicked, this, &MainWindow::start_game);
    connect(ui->quit, &QPushButton::clicked, this, &MainWindow::quit_program);
    connect(ui->create_map, &QPushButton::clicked, this, &MainWindow::create_map);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_game()
{
    Game::Get();
    this->close();
}


void MainWindow::quit_program()
{
    this->close();
}

void MainWindow::create_map()
{
    MapCreator::Get();
    this->close();
}

