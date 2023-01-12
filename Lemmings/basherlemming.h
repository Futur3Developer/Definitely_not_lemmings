#ifndef BASHERLEMMING_H
#define BASHERLEMMING_H

#include "lemming.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class BasherLemming: public Lemming{
    Q_OBJECT

public:
    BasherLemming();
    void move() override;

private:
    void resolve_collision(QList<QGraphicsItem*> colliding_items);
    void resolve_collision_with_block(QGraphicsItem *block);
    void resolve_collision_with_blocker(QGraphicsItem *blocker);
    void update_axis_speeds();
    void bounce_back();

    void bash_down_block(QGraphicsItem *block);
    int blocks_to_destroy = 3;
    double x_axis_speed_bonus_value = 3;
};

#endif // BASHERLEMMING_H
