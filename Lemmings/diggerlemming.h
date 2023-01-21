#ifndef DIGGERLEMMING_H
#define DIGGERLEMMING_H

#include "lemming.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class DiggerLemming: public Lemming{
    Q_OBJECT
public:
    DiggerLemming();
    void move() override;

    enum { Type = UserType + 8 };
    int type() const override { return Type; }

private:
    void resolve_collision(QList<QGraphicsItem*> colliding_items);
    void resolve_collision_with_block(QGraphicsItem *block);
    void resolve_collision_with_blocker(QGraphicsItem *blocker);
    void update_axis_speeds();
    void bounce_back();

    void start_digging();
    void digg_out_blocks_layer(QGraphicsItem *block);
    int digging_mode = 0;
    int block_layers_to_digg_out = 3;
    bool block_layer_was_digged_out = false;
};

#endif // DIGGERLEMMING_H
