#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsItem>
#include <QBrush>

//Block - building block of a map
class Block: public QGraphicsRectItem, QObject
{
public:
    Block();
    enum { Type = UserType + 1 };
    int type() const override { return Type; }
};

#endif // BLOCK_H
