#ifndef EXIT_H
#define EXIT_H

#include <QGraphicsItem>
#include <QBrush>

//Exit - part of the map, lemmings are supposed to get there
class Exit: public QGraphicsRectItem
{
public:
    Exit();

    enum { Type = UserType + 3 };

    int type() const override { return Type; }

    //TODO: Method for score update on lemmings collision
private:
};

#endif // EXIT_H
