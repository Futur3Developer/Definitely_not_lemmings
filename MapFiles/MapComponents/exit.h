#ifndef EXIT_H
#define EXIT_H

#include <QGraphicsItem>
#include <QBrush>

/**
 * Rescue point for lemmings. Required component of Map instance to start game.
 * Lemming colliding with it is equivalent to lemmings saved.
 */
class Exit: public QGraphicsRectItem
{
public:
    Exit();
    ~Exit();

    enum { Type = UserType + 3 };

    int type() const override { return Type; }

private:
};

#endif // EXIT_H
