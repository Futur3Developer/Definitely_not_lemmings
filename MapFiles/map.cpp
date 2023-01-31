#include "map.h"
#include "MapFiles/MapComponents/entrance.h"
#include "MapFiles/MapComponents/exit.h"

Map::Map()
{
    this -> setSceneRect(0,0,map_width,map_height);
    QPixmap background(":/graphics/background");
    setBackgroundBrush(background.scaled(map_width, map_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

int Map::get_width() const
{
    return map_width;
}

int Map::get_height() const
{
    return map_height;
}

int Map::get_gui_panel_boundary_y_pos() const
{
    return gui_panel_boundary_y_pos;
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
    entrance -> setZValue(10);
}

void Map::add_exit(int x_pos, int y_pos)
{
    Exit *exit = new Exit();
    exit -> setPos(x_pos, y_pos);

    addItem(exit);
}

void Map::add_gui_panel(int panel_height, int panel_y_pos)
{
    QGraphicsRectItem *gui_panel = new QGraphicsRectItem;
    gui_panel->setRect(0, 0, map_width, panel_height);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::lightGray);
    gui_panel->setBrush(brush);

    gui_panel -> setPos(0, panel_y_pos);
    this -> addItem(gui_panel);
}
