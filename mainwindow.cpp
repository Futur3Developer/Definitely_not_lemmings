#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MapFiles/mapcreator.h"
#include "MapFiles/MapConversion/mapconversionmanager.h"
#include "MapFiles/MapConversion/mapconverter.h"
#include "MapFiles/MapConversion/mapxmlconverter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    connect(ui->quit, &QPushButton::clicked, this, &MainWindow::quit_program);
    connect(ui->create_map, &QPushButton::clicked, this, &MainWindow::create_map);
    connect(ui->load_map, &QPushButton::clicked, this, &MainWindow::load_map);

    for (int i = 0; i < ui->levels_layout->count(); ++i)
    {
        QWidget *widget_in_layout = ui->levels_layout->itemAt(i)->widget();
        if(widget_in_layout != nullptr)
        {
            QPushButton *button_in_layout = dynamic_cast<QPushButton*>(widget_in_layout);
            connect(button_in_layout, &QPushButton::clicked, this, &MainWindow::start_game);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_game()
{
    QSharedPointer<QFile> map_file = assert_that_level_can_be_loaded();

    if(map_file == nullptr)
    {
        QMessageBox::warning(this, NULL, "Levels directory or particular map file could not be loaded."
                                         "Try loading another level.");
        return;
    }

    MapConverter *map_converter = new MapXMLconverter;
    Map* map = map_converter -> load_map_from_file(map_file);

    if(map == nullptr)
    {
        QMessageBox::warning(this, NULL, "Selected level could not be loaded."
                                         "Try loading another level.");
        return;
    }

    Game::Get().initialize_game(map);

    close_menu();
}


void MainWindow::quit_program()
{
    this->close();
}

void MainWindow::create_map()
{
    MapCreator *map_creator = new MapCreator(nullptr);
    map_creator -> show();

    close_menu();
}

void MainWindow::load_map()
{
    Map* map = MapConversionManager::Get().process_map_loading_request(this);

    if(map == nullptr)
        return;

    MapCreator *map_creator_instance = new MapCreator(map);
    map_creator_instance -> show();

    close_menu();
}

void MainWindow::close_menu()
{
    bool menu_is_closed = this -> close();

    if(!(menu_is_closed))
    {
        QMessageBox::critical(this, NULL, "Menu could not be closed.");
        return;
    }
}

QSharedPointer<QFile> MainWindow::assert_that_level_can_be_loaded()
{
    QString levels_directory_path = qApp -> applicationDirPath() + "/Levels";
    const QFileInfo levels_directory(levels_directory_path);

    if ((!levels_directory.exists()) || (!levels_directory.isDir()) || (!levels_directory.isWritable()))
    {
        QString warning = "Levels directory " + levels_directory_path + " could not be accessed.";
        QMessageBox::critical(this, NULL, warning);
        QSharedPointer<QFile> dummy_file = nullptr;
        return dummy_file;
    }

    QString sender_name = sender() -> objectName();
    QString selected_levels_path = levels_directory_path + "/" + sender_name + ".xml";
    QSharedPointer<QFile> map_file = QSharedPointer<QFile>(new QFile(selected_levels_path));

    if (!(map_file->open(QIODevice::ReadOnly)))
    {
        map_file -> close();
        QString warning = "Map file " + selected_levels_path + " could not be loaded";
        QMessageBox::warning(this, NULL, warning);
        QSharedPointer<QFile> dummy_file = nullptr;
        return dummy_file;
    }

    return map_file;
}

