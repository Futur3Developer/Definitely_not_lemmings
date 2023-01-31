#ifndef GAME_H
#define GAME_H

#include "Lemmings/lemming.h"
#include "MapFiles/MapComponents/entrance.h"
#include "MapFiles/MapComponents/exit.h"
#include "MapFiles/map.h"
#include "score.h"
#include "MapFiles/MapComponents/block.h"
#include "MapFiles/MapComponents/exit.h"
#include "mainwindow.h"

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
    void update_lemmings_alive_list(Lemming *lemming_to_delete, Lemming *lemming_to_update);

    void decrease_available_lemmings_class_changes(int class_index);
    void set_focused_lemming(Lemming *lemming);
    void set_main_window(MainWindow &main_menu);

    MainWindow *get_main_menu() const;
    Score *get_score_pointer() const;
    QList<Lemming*> *get_lemmings_alive();
    Lemming *get_focused_lemming() const;
    Map *get_map() const;
    int get_lemmings_to_save() const;
    int get_lemmings_to_spawn() const;
    int get_available_lemmings_class_changes(int class_index) const;
    QPushButton *get_class_changing_button(int button_index) const;
    QList<QPushButton*> class_changing_buttons;

private:
    Game();

    void keyPressEvent(QKeyEvent *event) override;
    void change_game_speed(QKeyEvent *event);

    /**
     * Locates Entrance in list of QGraphicsItems contained by Map instance used in current game.
     * Assign's pointer to it to private attribute entrance.
     */
    void locate_entrance_in_map();
    void set_view_configuration();
    /**
     * Starts the game by spawning JoblessLemmings equal to amount of lemmings_to_spawn through Entrance::spawn_lemming method.
     * Each spawned lemming is added to lemmings_alive list. After spawning all lemmings back_button and game_speed_changing_buttons are enabled.
     */
    void start_game();

    void add_GUI();
    void add_back_button();
    void add_lemming_class_changing_button(QPushButton *button, int button_index, QRect button_geometry, QString icon_path);
    void add_game_speed_changing_button(QString icon_path, int button_x_pos, int button_index);
    void add_graphics_text_item_center_aligned_to_button(QGraphicsTextItem *text_item, QPushButton *button, int vertical_offset_from_button, QString text, QFont font);
    void add_game_start_guideline();

    /**
     * Clears current game's data. Required because of singleton pattern usage.
     */
    void clear_current_game();
    void clear_current_games_GUI();

    int lemmings_to_spawn;
    int lemmings_to_save;
    const int default_game_speed = 1;
    int game_speed;
    bool lemmings_spawning_started = false;
    bool lemmings_were_spawned = false;
    const int default_step_interval = 20;
    int next_game_speed_button_x_pos = 10;

    QList<Lemming*> lemmings_alive;
    Map *map = nullptr;
    Entrance *entrance;
    QTimer *timer = nullptr;
    Score *score;
    Lemming *focused_lemming = nullptr;
    QPushButton *back_button;
    QList<QPushButton*> game_speed_changing_buttons;
    QList<QGraphicsTextItem*> lemmings_classes_names;
    QList<QGraphicsProxyWidget*> buttons_proxies;
    QList<QGraphicsTextItem*> available_lemmings_class_changes_as_text_items;
    QGraphicsTextItem *game_start_guideline;

    MainWindow *main_menu;

private slots:
    /**
     * Calls move() method for each instance of lemming in lemmings_alive list.
     * Slot is connected to timer's timeout signal.
     */
    void step();

    /**
     * Shows MainWindow to the user and hides Game instance.
     * Connected to back_button's clicked signal.
     */
    void back_to_main_menu();

    /**
     * Relays value corresponding to pressed class_changing_button as QKeyEvent to lemming's instance's pointed to by focused_lemming method keyPressEvent.
     * Connected to each button's in class_changing_buttons list clicked signal.
     * @param simulated_key_value
     */
    void relay_change_class_request_to_focused_lemming(int simulated_key_value);

    /**
     * Updates interval of timer based on @param:new_game_speed. In case of new_game_speed value of 0 it can also stop the timer completely.
     * Connected to each button's in game_speed_changing_buttonss list clicked signal.
     * @param new_game_speed - new game speed to be set, equal to index of the button emitting signal in list game_speed_changing_buttons.
     */
    void update_step_interval(int new_game_speed);
};


#endif // GAME_H

