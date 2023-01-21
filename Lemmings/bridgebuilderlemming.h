#ifndef BRIDGEBUILDERLEMMING_H
#define BRIDGEBUILDERLEMMING_H

#include "lemming.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class BridgeBuilderLemming: public Lemming{
    Q_OBJECT
public:
    BridgeBuilderLemming();
    void move() override;

    enum { Type = UserType + 9 };
    int type() const override { return Type; }

private:
    void resolve_collision(QList<QGraphicsItem*> colliding_items);
    void resolve_collision_with_block(QGraphicsItem *block);
    void resolve_collision_with_blocker(QGraphicsItem *blocker);
    void update_axis_speeds();
    void bounce_back();

    void respond_to_end_of_ground();
    void build();
    int blocks_to_build = 3;
};

#endif // BRIDGEBUILDERLEMMING_H
