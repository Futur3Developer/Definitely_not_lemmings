#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include "block.h"
#include "entrance.h"
#include "exit.h"

class Map : public QGraphicsScene
{
public:
    Map();

    int get_width() const;
    int get_height() const;

    void save_map_to_XML();
    void load_map_from_XML();

private:
    int map_width = 1760;
    int map_height = 960;

};

#endif // MAP_H
