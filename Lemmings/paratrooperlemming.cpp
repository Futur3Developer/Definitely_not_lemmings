#include "paratrooperlemming.h"
#include "blockerlemming.h"
#include "MapFiles/MapComponents/exit.h"
#include "game.h"

ParatrooperLemming::ParatrooperLemming()
{
    setPixmap(QPixmap(":/graphics/ParatrooperLemming_parachute_closed"));
    lemmings_height_without_parachute = this -> boundingRect().height() - 72;
}

void ParatrooperLemming::move()
{
    QList<QGraphicsItem*> colliding_items = this -> collidingItems();

    if(colliding_items.size() != 0)
    {
        resolve_collision(colliding_items);
        if(lemming_is_waiting_for_deletion)
            return;
    }
    update_axis_speeds();

    this -> setPos(pos().x() + x_axis_speed, pos().y()+ y_axis_speed);

    bool lemming_out_of_scene = check_for_lemming_out_of_scene();
    if(lemming_out_of_scene)
        safely_delete_lemming(false);

    largest_obstacle_height_prevalence = -1;
}

void ParatrooperLemming::resolve_collision(QList<QGraphicsItem*> colliding_items)
{
    for (int i = 0; i < colliding_items.size(); i++)
    {
        if (colliding_items[i] -> type() == Exit::Type)
        {
            resolve_collision_with_exit();
            return;
        }
        else if(colliding_items[i] -> type() == Block::Type && colliding_items[i] -> scene() != nullptr)
        {
            resolve_collision_with_block(colliding_items[i]);
        }
        else if(colliding_items[i] -> type() == BlockerLemming::Type)
            resolve_collision_with_blocker(colliding_items[i]);
    }
}

void ParatrooperLemming::bounce_back()
{
    x_axis_speed = -x_axis_speed;
    this -> setPixmap(this->pixmap().transformed(QTransform().scale(-1, 1)));
}

void ParatrooperLemming::open_parachute()
{
    if(x_axis_speed < 0)
        setPixmap(QPixmap(":/graphics/ParatrooperLemming_parachute_open").transformed(QTransform().scale(-1, 1)));
    else
        setPixmap(QPixmap(":/graphics/ParatrooperLemming_parachute_open"));

    parachute_is_open = true;
    y_axis_speed = default_y_axis_speed/2;
    fall_height_counter = 0;
}

void ParatrooperLemming::resolve_collision_with_block(QGraphicsItem *block)
{
    double height_difference = this -> pos().y() + boundingRect().height() - block -> pos().y();
    bool lemming_is_moving_in_direction_of_the_obstacle = check_for_lemming_moving_in_direction_of_the_obstacle(block);

    if(height_difference > this -> lemmings_height_without_parachute/2 && lemming_is_moving_in_direction_of_the_obstacle)
        bounce_back();
    else if(height_difference > largest_obstacle_height_prevalence)
        largest_obstacle_height_prevalence = height_difference;
}

void ParatrooperLemming::resolve_collision_with_blocker(QGraphicsItem *blocker)
{
    bool lemming_is_moving_in_direction_of_the_blocker = check_for_lemming_moving_in_direction_of_the_obstacle(blocker);
    if(lemming_is_moving_in_direction_of_the_blocker)
        bounce_back();
}

void ParatrooperLemming::update_axis_speeds()
{
    if(largest_obstacle_height_prevalence == -1)
    {
        if(!(parachute_is_open))
        {
            y_axis_speed = default_y_axis_speed;
            fall_height_counter += y_axis_speed;
        }

        if(fall_height_counter >= 160)
            open_parachute();
    }
    else
    {
        if(!(parachute_is_open))
        {
            y_axis_speed = 0;
            fall_height_counter = 0;

            if(largest_obstacle_height_prevalence <= this -> lemmings_height_without_parachute/2)
                y_axis_speed = -largest_obstacle_height_prevalence;

        }
        else
            fire_lemming();

    }
}
