#include "map.h"

Map::Map()
{
    this->setSceneRect(0,0,map_width,map_height);
}

int Map::get_width() const
{
    return map_width;
}

int Map::get_height() const
{
    return map_height;
}

void Map::save_map_to_XML()
{
    QList<QGraphicsItem*> components_list = this -> items(0, 0, map_width, map_height - 230, Qt::ContainsItemShape, Qt::DescendingOrder, QTransform());

    Q_FOREACH(const QGraphicsItem* component, components_list)
    {

    }
}

void Map::load_map_from_XML()
{

}



