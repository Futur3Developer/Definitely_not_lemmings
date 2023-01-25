#include "game.h"
#include "MapFiles/MapComponents/block.h"
#include "MapFiles/MapComponents/exit.h"
#include "mainwindow.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsItem>
#include <QDebug>

Game::Game():QGraphicsView(){}

void Game::add_game_start_guideline()
{
    game_start_guideline = new QGraphicsTextItem();
    game_start_guideline->setPlainText(QString("Press space to start"));
    game_start_guideline->setDefaultTextColor(Qt::darkGreen);
    game_start_guideline->setFont(QFont("times", 40));

    int x = map -> get_width()/2 - game_start_guideline -> boundingRect().width()/2;
    int y = map -> get_height()/2;

    game_start_guideline->setPos(x, y);
    map -> addItem(game_start_guideline);
}

void Game::initialize_game(Map *map)
{
    game_speed = default_game_speed;
    lemmings_spawning_started = false;
    lemmings_were_spawned = false;

    this -> map = map;
    map -> add_gui_panel(map -> get_height() - map -> get_gui_panel_boundary_y_pos(), map -> get_gui_panel_boundary_y_pos());
    lemmings_to_save = map -> lemmings_to_save;
    lemmings_to_spawn = map -> lemmings_to_spawn;

    set_view_configuration();

    score = new Score();
    int score_text_width = score -> boundingRect().width();
    score -> setPos(this -> map -> get_width() - score_text_width - 10, 10);
    map -> addItem(score);

    add_GUI();
    add_game_start_guideline();

    this -> show();
}

void Game::set_view_configuration()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(map -> get_width(), map -> get_height());
    setScene(map);
}

QList<Lemming*> *Game::get_lemmings_alive_pointer()
{
    return &lemmings_alive;
}

Map *Game::get_map() const
{
    return map;
}

int Game::get_lemmings_to_save() const
{
    return lemmings_to_save;
}

Score *Game::get_score_pointer() const
{
    return score;
}

void Game::update_list(Lemming *lemming_to_delete, Lemming *lemming_to_update)
{
    int i = lemmings_alive.indexOf(lemming_to_delete);
    lemmings_alive[i] = lemming_to_update;
}

void Game::set_focused_lemming(Lemming *lemming)
{
    focused_lemming = lemming;
}

Lemming *Game::get_focused_lemming() const
{
    return focused_lemming;
}

void Game::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        if(lemmings_spawning_started == false)
            start_game();
        else
            return;
    }
    else if((event->key() == Qt::Key_Q || event->key() == Qt::Key_E) && lemmings_were_spawned == true)
        change_game_speed(event);

    QGraphicsView::keyPressEvent(event);
}

//TODO: Stop entrance from spawning lemmings if someone goes back
void Game::start_game()
{
    map -> removeItem(game_start_guideline);
    delete game_start_guideline;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    timer -> start(default_step_interval);

    lemmings_spawning_started = true;
    locate_entrance_in_map();

    //Only the first lemming should spawn without delay
    entrance -> spawn_lemming(false);
    for(int i = 1; i < lemmings_to_spawn; i++)
    {
        if(timer == nullptr)
            return;

        entrance -> spawn_lemming(true);
    }

    lemmings_were_spawned = true;

}

void Game::change_game_speed(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Q)
    {
        if(game_speed > 0)
            game_speed --;
    }
    else
    {
        if(game_speed < 2)
            game_speed ++;
    }

    game_speed_changing_buttons[game_speed] -> animateClick();
    update_step_interval(game_speed);
}

void Game::add_back_button()
{
    QGraphicsProxyWidget *proxy;
    back_button = new QPushButton;
    back_button -> setGeometry(QRect(50, map -> get_height() - 90, 100, 50));
    back_button -> setText("Back");

    proxy = map -> addWidget(back_button);
    connect(back_button, &QPushButton::clicked, this, &Game::back_to_main_menu);
    buttons_proxies.append(proxy);
}

void Game::add_GUI()
{
    add_back_button();

    QList<QString> game_speed_icons_paths_list = {":/graphics_icons/game_speed_0_icon", ":/graphics_icons/game_speed_1_icon",
                                                  ":/graphics_icons/game_speed_2_icon"};

    next_game_speed_button_x_pos = 10;
    for(int i = 0; i < game_speed_icons_paths_list.length(); i++)
        add_game_speed_changing_button(game_speed_icons_paths_list[i], next_game_speed_button_x_pos, i);

    int back_button_x_pos = back_button -> pos().x();
    int side_spacer = 50;
    int inner_spacer = 20;

    int lemmings_classes_implemented = 7;
    int total_space_available = map -> get_width() - back_button_x_pos - back_button -> width() - side_spacer*2 - inner_spacer*(lemmings_classes_implemented - 1);
    int single_button_width = total_space_available/lemmings_classes_implemented;
    int initial_button_x_pos = back_button_x_pos + back_button -> width() + side_spacer;
    int button_y_pos = map -> get_height() - 160;

    QList<QString> lemmings_classes_icons_paths_list = {":/graphics_icons/Blocker_icon", ":/graphics_icons/Climber_icon",
                                                        ":/graphics_icons/Paratrooper_icon", ":/graphics_icons/Basher_icon",
                                                        ":/graphics_icons/Digger_icon", ":/graphics_icons/BridgeBuilder_icon",
                                                        ":/graphics_icons/RampBuilder_icon"};

    for(int i = 0; i < lemmings_classes_icons_paths_list.length(); i++)
    {
        int current_button_x_pos = initial_button_x_pos + i*(single_button_width + inner_spacer);
        QRect button_geometry = QRect(current_button_x_pos, button_y_pos, single_button_width, 120);
        QString icon_path = lemmings_classes_icons_paths_list[i];

        QPushButton *button = new QPushButton();
        add_lemming_class_changing_button(button, i, button_geometry, icon_path);

        if(map -> available_lemmings_class_changes_list[i] == 0)
            button -> setDisabled(true);

        QStringList icon_path_splitted = lemmings_classes_icons_paths_list[i].split("/");
        QString icon_name = icon_path_splitted.value(icon_path_splitted.length() - 1);

        QString lemming_class_name = icon_name.split("_").first();
        QGraphicsTextItem *lemming_class_name_text_item = new QGraphicsTextItem();
        int font_size = 14;
        QFont font = QFont("times", font_size, QFont::Bold);
        int vertical_offset_from_button = -35;

        add_graphics_text_item_center_aligned_to_button(lemming_class_name_text_item, button,
                                                        vertical_offset_from_button, lemming_class_name, font);
        lemmings_classes_names.append(lemming_class_name_text_item);

        QGraphicsTextItem *available_class_changes_amount_text_item = new QGraphicsTextItem();
        QString available_class_changes_amount_text = QString::number(map -> available_lemmings_class_changes_list[i]);
        vertical_offset_from_button = button -> height() + 5;

        add_graphics_text_item_center_aligned_to_button(available_class_changes_amount_text_item, button,
                                                        vertical_offset_from_button, available_class_changes_amount_text, font);
        available_class_changes_amount_list.append(available_class_changes_amount_text_item);
    }
}

void Game::add_lemming_class_changing_button(QPushButton *button, int button_index, QRect button_geometry, QString icon_path)
{
    button -> setGeometry(button_geometry);

    QPixmap icon_pixmap(icon_path);
    QIcon button_icon(icon_pixmap);

    button -> setIcon(button_icon);
    button -> setIconSize(icon_pixmap.rect().size());

    int event_key_1_value = 49;
    int simulated_key_value = event_key_1_value + button_index;

    connect(button, &QPushButton::clicked, this, [=]{relay_change_class_request_to_focused_lemming(simulated_key_value); });
    class_changing_buttons.append(button);

    QGraphicsProxyWidget *proxy;
    proxy = map -> addWidget(button);
    buttons_proxies.append(proxy);
}

void Game::add_game_speed_changing_button(QString icon_path, int button_x_pos, int button_index)
{
    QPushButton *game_speed_button = new QPushButton;
    QPixmap icon_pixmap(icon_path);
    icon_pixmap = icon_pixmap.scaled(QSize(50, 50));
    QIcon button_icon(icon_pixmap);

    game_speed_button -> setIcon(button_icon);
    game_speed_button -> setIconSize(icon_pixmap.rect().size());
    QGraphicsProxyWidget *proxy;

    proxy = map -> addWidget(game_speed_button);
    proxy -> setPos(button_x_pos, 10);
    buttons_proxies.append(proxy);

    connect(game_speed_button, &QPushButton::clicked, this, [=]{update_step_interval(button_index); });

    game_speed_changing_buttons.append(game_speed_button);
    next_game_speed_button_x_pos = next_game_speed_button_x_pos + game_speed_button -> width() + 10;
}

void Game::add_graphics_text_item_center_aligned_to_button(QGraphicsTextItem *text_item, QPushButton *button, int vertical_offset_from_button, QString text, QFont font)
{
    text_item -> setPlainText(text);
    text_item -> setFont(font);
    int text_item_width = text_item -> boundingRect().width();

    int side_space_available = button -> width() - text_item_width;
    int text_item_side_spacer = side_space_available/2;

    text_item -> setPos(button -> pos().x() + text_item_side_spacer, button -> pos().y() + vertical_offset_from_button);
    map -> addItem(text_item);
}

int Game::get_lemmings_to_spawn() const
{
    return lemmings_to_spawn;
}

void Game::update_step_interval(int new_game_speed)
{
    game_speed = new_game_speed;
    if(game_speed == 0)
        timer -> stop();
    else
    {
        int interval = default_step_interval/game_speed;

        if(timer -> isActive())
            timer -> setInterval(interval);
        else
            timer -> start(interval);
    }
}

void Game::locate_entrance_in_map()
{
    QList<QGraphicsItem*> map_components = map -> items();

    foreach(const QGraphicsItem* component, map_components)
        if(component -> type() == Entrance::Type)
        {
            entrance = dynamic_cast<Entrance*>(map -> itemAt(component -> pos(), QTransform()));
            entrance -> setZValue(-2);
        }
}

void Game::step()
{
    for(int i = 0; i < lemmings_alive.size(); i++)
    {
        if(lemmings_alive[i] != nullptr)
            lemmings_alive[i] -> move();
    }
}

void Game::back_to_main_menu()
{
    clear_current_game();

    bool game_is_closed = this -> close();
    if(!(game_is_closed))
    {
        QMessageBox::critical(this, NULL, "Game could not be closed.");
        return;
    }

    MainWindow *menu = new MainWindow;
    menu -> show();
}

void Game::relay_change_class_request_to_focused_lemming(int simulated_key_value)
{
    if(focused_lemming == nullptr || focused_lemming -> accept_class_change == false)
        return;

    QKeyEvent *key_to_simulate = new QKeyEvent(QKeyEvent::KeyPress, simulated_key_value, Qt::NoModifier);
    focused_lemming -> keyPressEvent(key_to_simulate);
}

void Game::clear_current_game()
{
    //TODO: Check for memory leaks, assert that map can be deleted
    if(timer != nullptr)
    {
        timer -> stop();
        disconnect(timer, SIGNAL(timeout()), this, SLOT(step()));
        delete timer;
        timer = nullptr;
    }

    foreach(QGraphicsProxyWidget *item, buttons_proxies)
    {
        map -> removeItem(item);
    }

    buttons_proxies.clear();
    class_changing_buttons.clear();

    QList<QGraphicsTextItem *> temp_list;
    temp_list.append(available_class_changes_amount_list);
    temp_list.append(lemmings_classes_names);

    foreach(QGraphicsTextItem* item, temp_list)
    {
        QGraphicsItem *temp_item = qgraphicsitem_cast<QGraphicsItem*>(item);
        map -> removeItem(temp_item);

    }
    available_class_changes_amount_list.clear();
    lemmings_classes_names.clear();

    QGraphicsItem *temp_item = qgraphicsitem_cast<QGraphicsItem*>(score);
    map -> removeItem(temp_item);

    if(lemmings_alive.isEmpty() == false)
        lemmings_alive.clear();

//    Map *map_to_delete = this -> map;

//    Map *new_map = new Map();
//    this -> setScene(new_map);
//    this -> map = new_map;

//    delete map_to_delete;
}
