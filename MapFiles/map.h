#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>

#include "Lemmings/lemming.h"
#include "MapFiles/MapComponents/block.h"

class Map : public QGraphicsScene
{
public:
    Map();

    int get_width() const;
    int get_height() const;
    int get_gui_boundary_height() const;

    void add_block(int x_pos, int y_pos);
    void add_entrance(int x_pos, int y_pos);
    void add_exit(int x_pos, int y_pos);
    void add_item(QGraphicsItem *item, int x_pos, int y_pos);

private:
    void add_gui_panel_boundary();

    int map_width = 1760;
    int map_height = 960;
    int gui_boundary_height = 210;
};

#endif // MAP_H
