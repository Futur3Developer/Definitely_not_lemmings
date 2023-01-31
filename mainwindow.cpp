#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    connect(ui->quit, &QPushButton::clicked, this, &MainWindow::quit_program);
    connect(ui->create_map, &QPushButton::clicked, this, &MainWindow::create_map);
    connect(ui->load_map, &QPushButton::clicked, this, &MainWindow::load_map);
    connect(ui->informations, &QPushButton::clicked, this, &MainWindow::show_informations);

    for (int i = 0; i < ui->levels_layout->count(); i++)
    {
        QWidget *widget_in_layout = ui->levels_layout->itemAt(i)->widget();
        if(widget_in_layout != nullptr)
        {
            QPushButton *button_in_layout = dynamic_cast<QPushButton*>(widget_in_layout);
            connect(button_in_layout, &QPushButton::clicked, this, &MainWindow::start_game);
        }
    }

    map_converter = new MapXMLconverter();
}

MainWindow::~MainWindow()
{
    delete map_converter;
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

    Map* map = map_converter -> load_map_from_file(map_file);

    if(map == nullptr)
    {
        QMessageBox::warning(this, NULL, "Selected level could not be loaded."
                                         "Try loading another level.");
        return;
    }

    Game::Get().initialize_game(map);
    this -> hide();
}


void MainWindow::quit_program()
{
    this->close();
}

void MainWindow::create_map()
{
    MapCreator *map_creator = new MapCreator(nullptr);
    map_creator -> show();
    this -> hide();
}

void MainWindow::load_map()
{
    Map* map = MapConversionManager::Get().process_map_loading_request(this);

    if(map == nullptr)
        return;

    MapCreator *map_creator_instance = new MapCreator(map);
    map_creator_instance -> show();
    this -> hide();
}

void MainWindow::show_informations()
{
    QMessageBox::information(this, "Game informations and settings",
                             tr("Creating map:\n"
                                "- drag and drop map components to create desired layout\n"
                                "- use right mouse button while holding component to drop it back to its earlier position\n"
                                "- use middle mouse button to remove component from map.\n"
                                "- saving map or starting the game is only posibble when required components, entrance and exit, are placed.\n"
                                "...personally I would advice to use some blocks as well.\n"
                                "- map saving or starting the game requires to fill lemmings configuration, how many class changes are permitted,\n"
                                " how many lemmings should be spawned and how many of them need to be saved in order to win.\n"
                                "\n"
                                "Game:\n"
                                "Main goal of the game is to rescue required amount of spawned lemmings by guiding them from entrance to exit.\n"
                                "By default lemmings move right and bounce back after colliding with walls.\n"
                                "Lemming can die by falling out of the game map or falling from height of at least four blocks.\n"
                                "- both visible buttons and keyboard shortcuts can be used to play\n"
                                "- to change game speed to get more time to think or accelerate lemmings when it is certain they will pass the exit\n"
                                "use buttons in upper left corner or keys Q and E\n"
                                "- to change class of lemming selected with left mouse button use keys 1 to 7 or corresponding buttons in lower game panel.\n"
                                "  Be careful, if you change lemming's class he can only go back to being default lemming on his own after performing his duties.\n"
                                "  key 1: Blocker - lemming stops moving and blocks other lemmings from moving through him.\n"
                                "         Warning! Such a lemming can no longer change back to default one.\n"
                                "  key 2: Climber - lemming which instead of bouncing back from wall will start climbing it.\n"
                                "         After there is no longer need to climb, or he hits a ceiling, he will go back to being defualt lemming.\n"
                                "  key 3: Paratrooper - lemming which does not die from falling. If he deems it necessary (he has already fallen from height of two blocks)\n"
                                "         he will open parachute. He goes back to being default lemming after touching the ground with open parachute.\n"
                                "  key 4: Basher - lemming which does not let itself be intimidated by walls into bouncing back. Instead, he charges at them and destroys them.\n"
                                "         Like, maybe not all of them. His head would hurt as hell. Destroying three blocks is already a good deal.\n"
                                "  key 5: Digger - because of Basher's lemming's difficult working conditions lemmings unionized.\n"
                                "         Digger decided that he will also digg out three block's layers... and then go for a break. Meaning, he will go back to being default lemming.\n"
                                "  key 6: BridgeBuilder - He builds bridges. Yes, that's it. Of course he only builds them from three blocks and then goes for a break.\n"
                                "         At least you don't have to worry about changing class at perfect time. He will start building if ground under his feet ends.\n"
                                "  key 7: RampBuilder - He builds ramps. To be exact, he builds really bumpy ramps, consisting of THREE blocks placed diagonally.\n"
                                "\n"
                                "Good luck  :)"), QMessageBox::Ok);
}

QSharedPointer<QFile> MainWindow::assert_that_level_can_be_loaded()
{
    QString levels_directory_path = qApp -> applicationDirPath() + "/Levels";
    const QFileInfo levels_directory(levels_directory_path);

    if ((!levels_directory.exists()) || (!levels_directory.isDir()) || (!levels_directory.isWritable()))
    {
        QSharedPointer<QFile> dummy_file = nullptr;
        return dummy_file;
    }

    QString sender_name = sender() -> objectName();
    QString selected_levels_path = levels_directory_path + "/" + sender_name + ".xml";
    QSharedPointer<QFile> map_file = QSharedPointer<QFile>(new QFile(selected_levels_path));

    if (!(map_file->open(QIODevice::ReadOnly)))
    {
        map_file -> close();
        QSharedPointer<QFile> dummy_file = nullptr;
        return dummy_file;
    }

    return map_file;
}

