#ifndef BLOCKERLEMMING_H
#define BLOCKERLEMMING_H

#include "lemming.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class BlockerLemming: public Lemming{
    Q_OBJECT
public:
    BlockerLemming();
    enum { Type = UserType + 4 };
    int type() const override { return Type; }
    void move() override;

private:
    void resolve_collision(QList<QGraphicsItem*> colliding_items);
    void resolve_collision_with_block(QGraphicsItem *block);
    void resolve_collision_with_blocker(QGraphicsItem *blocker);
    void update_axis_speeds();
    void bounce_back();

    void start_blocking();
    int blocking_mode = 0;

};

#endif // BLOCKERLEMMING_H
