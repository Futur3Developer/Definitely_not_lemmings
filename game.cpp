#include "game.h"
#include "MapFiles/MapComponents/block.h"
#include "MapFiles/MapComponents/exit.h"

#include <QGraphicsItem>
#include <QDebug>

Game::Game():QGraphicsView()
{
    //TODO: GUI at the bottom of the map providing ability to change classes of lemmings
    //TODO: Value of lemmings_to_spawn and lemmings_to_save should be set by user.

    lemmings_to_spawn = 5;
    lemmings_to_save = 2;
}

void Game::initialize_game(Map *map)
{
    this -> map = map;
    set_view_configuration();
    score = new Score;
    int score_text_width = score -> boundingRect().width();
    score -> setPos(this -> map -> get_width() - score_text_width - 10, 0 + 10);
    map -> addItem(score);
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

void Game::start_game()
{
    lemmings_spawning_started = true;
    locate_entrance_in_map();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    timer -> start(default_step_interval);

    entrance -> spawn_lemming(false); //Only the first lemming should spawn without delay
    for(int i = 1; i < lemmings_to_spawn; i++)
        entrance -> spawn_lemming(true);
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
        if(game_speed < 3)
            game_speed ++;
    }

    update_step_interval();
}

int Game::get_lemmings_to_spawn() const
{
    return lemmings_to_spawn;
}

void Game::update_step_interval()
{
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

    Q_FOREACH(const QGraphicsItem* component, map_components)
        if(component -> type() == Entrance::Type)
            entrance = dynamic_cast<Entrance*>(map -> itemAt(component -> pos(), QTransform()));
}

void Game::step()
{
    for(int i = 0; i < lemmings_alive.size(); i++)
    {
        if(lemmings_alive[i] == NULL)
            continue;

        lemmings_alive[i] -> move();
    }
}
