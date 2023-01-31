#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsItem>
#include <QBrush>

/**
 * Basic component of Map instance. Used for designing game levels.
 */
class Block: public QGraphicsRectItem, QObject
{
public:
    Block();
    ~Block();

    enum { Type = UserType + 1 };
    int type() const override { return Type; }
};

#endif // BLOCK_H
