#ifndef ENTRANCE_H
#define ENTRANCE_H

#include <QGraphicsItem>
#include <QBrush>

//Entrance - part of the map, point of lemmings spawn
class Entrance: public QGraphicsRectItem
{
public:
    Entrance();

    enum { Type = UserType + 2 };

    int type() const override { return Type; }

    //TODO: Method for lemmings spawnning

    //void spawn_lemmings_on_keyPressEvent(QKeyEvent *event);

private:
    bool active = false; // For checking if it can spawn lemmings - if it's map creation, it can't
};

#endif // ENTRANCE_H
