#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>

#include "Lemmings/lemming.h"
#include "MapFiles/MapComponents/block.h"

//Level would be propably better class name. Even better, make a new class Level with lemmings configuration and Map as one of attributes
class Map : public QGraphicsScene
{
public:
    Map();
    virtual ~Map() = default;

    //Methods for adding items in MapCreator
    void add_block(int x_pos, int y_pos);
    void add_entrance(int x_pos, int y_pos);
    void add_exit(int x_pos, int y_pos);
    void add_item(QGraphicsItem *item, int x_pos, int y_pos);

    //GUI initialization methods for use in MapCreator or Game classes
    void add_gui_panel(int panel_height, int panel_y_pos);

    //Methods for getting basic scene attributes
    int get_width() const;
    int get_height() const;
    int get_gui_panel_boundary_y_pos() const;

    //Attributes for level's lemmings configuration
    QList<int> available_lemmings_class_changes_list;
    int lemmings_to_spawn;
    int lemmings_to_save;

private:
    const int map_width = 1920;
    const int map_height = 960;
    const int gui_panel_boundary_y_pos = 750;
};

#endif // MAP_H
