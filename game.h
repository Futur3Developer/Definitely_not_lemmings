#ifndef GAME_H
#define GAME_H

#include "Lemmings/lemming.h"
#include "MapFiles/MapComponents/entrance.h"
#include "MapFiles/MapComponents/exit.h"
#include "MapFiles/map.h"
#include "score.h"

#include <QPushButton>
#include <QMessageBox>
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

    //ONLY FOR TEST PURPOSES THIS IS PUBLIC OR EVEN CREATED - TODO: CLEAR THIS UP
    QList<QPushButton*> class_changing_buttons;
    QList<QGraphicsProxyWidget*> buttons_proxies;
    QList<QGraphicsTextItem*> available_class_changes_amount_list;
    QList<QGraphicsTextItem*> lemmings_classes_names;

private:
    Game();

    //Methods for handling QKeyEvent
    void keyPressEvent(QKeyEvent *event) override;
    void change_game_speed(QKeyEvent *event);

    //Methods for preparing and starting game
    void locate_entrance_in_map();
    void set_view_configuration();
    void start_game();

    //GUI initialization methods
    void add_GUI();
    void add_back_button();
    void add_lemming_class_changing_button(QPushButton *button, int button_index, QRect button_geometry, QString icon_path);
    void add_game_speed_changing_button(QString icon_path, int button_x_pos, int button_index);
    void add_graphics_text_item_center_aligned_to_button(QGraphicsTextItem *text_item, QPushButton *button, int vertical_offset_from_button, QString text, QFont font);
    void add_game_start_guideline();

    //Basic game attributes
    int lemmings_to_spawn;
    int lemmings_to_save;
    const int default_game_speed = 1;
    int game_speed;
    bool lemmings_spawning_started = false;
    bool lemmings_were_spawned = false;
    const int default_step_interval = 20;

    QList<Lemming*> lemmings_alive;
    Map *map;
    Entrance *entrance;
    QTimer *timer = nullptr;
    Score *score;
    Lemming *focused_lemming = nullptr;
    QPushButton *back_button;
    QList<QPushButton*> game_speed_changing_buttons;
    QGraphicsTextItem *game_start_guideline;
    int next_game_speed_button_x_pos = 10;

    //Method for clearing current game's data in case someone wants to play again
    void clear_current_game();

private slots:
    ///Main slot calling move() method for all lemmings
    void step();

    //Slots connected to buttons
    void back_to_main_menu();
    void relay_change_class_request_to_focused_lemming(int simulated_key_value);
    void update_step_interval(int new_game_speed);
};


#endif // GAME_H

