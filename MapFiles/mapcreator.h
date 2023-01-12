#ifndef MAPCREATOR_H
#define MAPCREATOR_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QMessageBox>

#include "MapFiles/MapComponents/block.h"
#include "MapComponents/exit.h"
#include "MapComponents/entrance.h"
#include "map.h"

class MapCreator: public QGraphicsView
{
    Q_OBJECT
public:
    MapCreator();
    MapCreator(Map *map);

    Map *get_map();
    void set_map(Map* map);

private:
    void set_view_configuration();
    void initialize_map_components(bool map_is_being_edited);
    void add_guideline(QString text, int x_pos, int y_pos);
    void add_map_creation_guidelines();
    void add_button(QPushButton *button, int aleft, const QString &text);
    void add_buttons();

    void mousePressEvent(QMouseEvent *event);
    void pick_up_component(QGraphicsItem *component);
    void mouseMoveEvent(QMouseEvent *event);

    void restore_component_to_latest_position();
    void place_component(QPointF new_position);
    bool assert_that_position_is_valid(QPointF new_position);
    bool check_if_there_is_new_block_to_place();
    void remove_component_from_map();
    Map *prepare_map_without_gui();

    bool assert_that_required_components_are_placed();

    int gui_boundary_height = 210;
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
    void request_map_saving();
    void start_game();
};
#endif // MAPCREATOR_H
