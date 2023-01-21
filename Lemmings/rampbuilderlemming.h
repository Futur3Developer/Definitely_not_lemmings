#ifndef RAMPBUILDERLEMMING_H
#define RAMPBUILDERLEMMING_H

#include "lemming.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class RampBuilderLemming: public Lemming{
    Q_OBJECT
public:
    RampBuilderLemming();
    void move() override;

    enum { Type = UserType + 10 };
    int type() const override { return Type; }

private:
    void resolve_collision(QList<QGraphicsItem*> colliding_items);
    void resolve_collision_with_block(QGraphicsItem *block);
    void resolve_collision_with_blocker(QGraphicsItem *blocker);
    void update_axis_speeds();
    void bounce_back();

    void check_for_farther_building_instructions();
    void build();
    bool building_started = false;
    int blocks_to_build = 3;
    double last_block_built_pos_x = 0;
};

#endif // RAMPBUILDERLEMMING_H
