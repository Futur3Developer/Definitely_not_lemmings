#ifndef MAPCREATOR_H
#define MAPCREATOR_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>

#include <QPushButton>
#include <QMessageBox>

#include "block.h"
#include "exit.h"
#include "entrance.h"
#include "map.h"

class MapCreator: public QGraphicsView
{
    Q_OBJECT
public:
    MapCreator(const MapCreator&) = delete;

    static MapCreator& Get()
    {
        static MapCreator map_creator_Instance;
        return map_creator_Instance;
    }

private:
    MapCreator();

    void set_view_configuration();
    void initialize_map_components();

    void add_gui_panel_boundary();
    void add_entrance();
    void add_exit();
    void add_block();
    void add_guideline(QString text, int x_pos, int y_pos);
    void add_map_creation_guidelines();
    void add_button(QPushButton *button, int aleft, const QString &text);
    void add_buttons();

    void mousePressEvent(QMouseEvent *event); //Actions on components
    void pick_up_component(QGraphicsItem *component);
    void mouseMoveEvent(QMouseEvent *event); //Moving components

    void restore_component_to_latest_position();
    void place_component(QPointF new_position);
    bool assert_that_position_is_valid(QPointF new_position);
    bool check_if_there_is_new_block_to_place();
    void remove_component_from_map();

    //void handle_buttons_mouse_press_events(QGraphicsItem *clicked_button);
    bool assert_that_required_components_are_placed();

    Map *map;

    QPointF entrance_original_position;
    QPointF exit_original_position;
    QPointF block_original_position;
    QPointF component_original_position;

    bool component_is_being_placed = false;
    QGraphicsItem *component_to_place;

    QPushButton *back_button;
    QPushButton *save_map_button;
    QPushButton *play_button;

private slots:
    void back_to_main_menu();
    void save_map_to_XML();
    void start_game();



};
#endif // MAPCREATOR_H
