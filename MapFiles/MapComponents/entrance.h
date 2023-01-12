#ifndef ENTRANCE_H
#define ENTRANCE_H

#include <QGraphicsItem>
#include <QBrush>
#include <QKeyEvent>

//Entrance - part of the map, point of lemmings spawn
class Entrance: public QGraphicsRectItem
{
public:
    Entrance();

    enum { Type = UserType + 2 };

    int type() const override { return Type; }
    void spawn_lemming(bool delay_should_be_applied);
    void delay_spawn(int delay_time_in_milliseconds);

private:

};

#endif // ENTRANCE_H
