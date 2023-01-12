#include "basherlemming.h"
#include "blockerlemming.h"
#include "MapFiles/MapComponents/exit.h"
#include "game.h"

BasherLemming::BasherLemming()
{
    setPixmap(QPixmap(":/graphics/BasherLemming"));
}

void BasherLemming::move()
{
    QList<QGraphicsItem*> colliding_items = this -> collidingItems();

    if(colliding_items.size() != 0)
    {
        resolve_collision(colliding_items);
        if(lemming_is_waiting_for_deletion)
            return;
    }
    int old_y_axis_speed = y_axis_speed;
    update_axis_speeds();

    if(old_y_axis_speed != y_axis_speed && fall_height_counter >= death_fall_height)
    {
        safely_delete_lemming(false);
        return;
    }

    this -> setPos(pos().x() + x_axis_speed + x_axis_speed_bonus_value, pos().y()+ y_axis_speed);

    bool lemming_out_of_scene = check_for_lemming_out_of_scene();
    if(lemming_out_of_scene)
        safely_delete_lemming(false);

    largest_obstacle_height_prevalence = -1;

    if(x_axis_speed > 0)
        x_axis_speed_bonus_value = x_axis_speed_bonus_value + 0.2;
    else
        x_axis_speed_bonus_value = x_axis_speed_bonus_value - 0.2;
}

void BasherLemming::resolve_collision(QList<QGraphicsItem*> colliding_items)
{
    for (int i = 0; i < colliding_items.size(); i++)
    {
        if (colliding_items[i] -> type() == Exit::Type)
        {
            resolve_collision_with_exit();
            return;
        }
        else if(colliding_items[i] -> type() == Block::Type && colliding_items[i] -> scene() != NULL)
        {
            resolve_collision_with_block(colliding_items[i]);
        }
        else if(colliding_items[i] -> type() == BlockerLemming::Type)
            resolve_collision_with_blocker(colliding_items[i]);
    }
}

void BasherLemming::bounce_back()
{
    x_axis_speed = -x_axis_speed;
    x_axis_speed_bonus_value = -x_axis_speed_bonus_value;
    this -> setPixmap(this->pixmap().transformed(QTransform().scale(-1, 1)));
}

void BasherLemming::bash_down_block(QGraphicsItem *block)
{
    block -> hide();
    delete block;
    this -> setPos(pos().x() - 5*x_axis_speed, pos().y());
    blocks_to_destroy --;
}

void BasherLemming::resolve_collision_with_block(QGraphicsItem *block)
{
    double height_difference = this -> pos().y() + boundingRect().height() - block -> pos().y();
    bool lemming_is_moving_in_direction_of_the_obstacle = check_for_lemming_moving_in_direction_of_the_obstacle(block);

    if(height_difference > this -> boundingRect().height()/2 && lemming_is_moving_in_direction_of_the_obstacle)
    {
        x_axis_speed_bonus_value = 0;

        if(blocks_to_destroy > 0)
            bash_down_block(block);

        if(blocks_to_destroy == 0)
            fire_lemming();
    }
    else if(height_difference > largest_obstacle_height_prevalence)
        largest_obstacle_height_prevalence = height_difference;
}

void BasherLemming::resolve_collision_with_blocker(QGraphicsItem *blocker)
{
    bool lemming_is_moving_in_direction_of_the_blocker = check_for_lemming_moving_in_direction_of_the_obstacle(blocker);
    if(lemming_is_moving_in_direction_of_the_blocker)
        bounce_back();
}

void BasherLemming::update_axis_speeds()
{
    if(largest_obstacle_height_prevalence == -1)
    {
        x_axis_speed_bonus_value = 0;
        y_axis_speed = default_y_axis_speed;
        fall_height_counter += y_axis_speed;
    }
    else
    {
        y_axis_speed = 0;

        if(fall_height_counter <death_fall_height)
            fall_height_counter = 0;

        if(largest_obstacle_height_prevalence <= this -> boundingRect().height()/2)
            y_axis_speed = -largest_obstacle_height_prevalence;
    }
}
