#ifndef CLIMBERLEMMING_H
#define CLIMBERLEMMING_H

#include "lemming.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QObject>

class ClimberLemming: public Lemming{
    Q_OBJECT
public:
    ClimberLemming();
    void move() override;
    enum { Type = UserType + 5 };
    int type() const override { return Type; }

private:
    void resolve_collision(QList<QGraphicsItem*> colliding_items);
    void resolve_collision_with_block(QGraphicsItem *block);
    void resolve_collision_with_blocker(QGraphicsItem *blocker);
    void update_axis_speeds();
    bool check_for_lemming_moving_in_direction_of_the_obstacle(QGraphicsItem *obstacle);
    void bounce_back();

    void start_climbing();
    void stop_climbing();
    int climbing_mode = 0;
    int x_axis_versor_before_climbing_started = 1;
};

#endif // CLIMBERLEMMING_H
