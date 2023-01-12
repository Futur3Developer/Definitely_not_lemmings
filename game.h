#ifndef GAME_H
#define GAME_H

#include "Lemmings/lemming.h"
#include "MapFiles/MapComponents/entrance.h"
#include "MapFiles/MapComponents/exit.h"
#include "MapFiles/map.h"
#include "score.h"

#include <QTimer>
#include <QGraphicsView>
#include <QKeyEvent>

class Game: public QGraphicsView{
    Q_OBJECT
public:
    Game(const Game&) = delete;

    static Game& Get()
    {
        static Game Game_Instance;
        return Game_Instance;
    }

    void initialize_game(Map *map);
    void update_list(Lemming *lemming_to_delete, Lemming *lemming_to_update);
    void set_focused_lemming(Lemming *lemming);

    QList<Lemming*> *get_lemmings_alive_pointer();
    Lemming *get_focused_lemming() const;
    Map *get_map() const;

    Score *get_score_pointer() const;

    int get_lemmings_to_save() const;
    int get_lemmings_to_spawn() const;

private:
    Game();

    void keyPressEvent(QKeyEvent *event) override;
    void locate_entrance_in_map();
    void set_view_configuration();
    void start_game();
    void update_step_interval();
    void change_game_speed(QKeyEvent *event);

    QList<Lemming*> lemmings_alive;
    Map *map;
    Entrance *entrance;
    QTimer *timer;
    Score *score;
    Lemming* focused_lemming = NULL;

    int lemmings_to_spawn;
    int lemmings_to_save;
    int game_speed = 1;
    bool lemmings_spawning_started = false;
    bool lemmings_were_spawned = false;
    const int default_step_interval = 20;

private slots:
    void step();


};


#endif // GAME_H

