#ifndef JOBLESSLEMMING_H
#define JOBLESSLEMMING_H

#include "lemming.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class JoblessLemming: public Lemming{
    Q_OBJECT
public:
    JoblessLemming();
    void move() override;

private:
    void resolve_collision(QList<QGraphicsItem*> colliding_items);
    void resolve_collision_with_block(QGraphicsItem *block);
    void resolve_collision_with_blocker(QGraphicsItem *blocker);
    void update_axis_speeds();
    void bounce_back();
};

#endif // JOBLESSLEMMING_H
