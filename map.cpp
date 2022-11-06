#include "map.h"

Map::Map()
{
    this -> setSceneRect(0,0,map_width,map_height);
    add_gui_panel_boundary();
}

int Map::get_width() const
{
    return map_width;
}

int Map::get_height() const
{
    return map_height;
}

int Map::get_gui_boundary_height() const
{
    return gui_boundary_height;
}


void Map::add_gui_panel_boundary()
{
    QGraphicsLineItem *boundary = new QGraphicsLineItem(0, map_height - gui_boundary_height, map_width, map_height - gui_boundary_height);
    addItem(boundary);
}

void Map::add_block(int x_pos, int y_pos)
{
    Block *block = new Block();
    block -> setPos(x_pos, y_pos);

    addItem(block);
}

void Map::add_entrance(int x_pos, int y_pos)
{
    Entrance *entrance = new Entrance();
    entrance -> setPos(x_pos, y_pos);

    addItem(entrance);
}

void Map::add_exit(int x_pos, int y_pos)
{
    Exit *exit = new Exit();
    exit -> setPos(x_pos, y_pos);

    addItem(exit);
}

