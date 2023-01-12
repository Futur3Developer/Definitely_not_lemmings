#include "rampbuilderlemming.h"
#include "blockerlemming.h"
#include "MapFiles/MapComponents/exit.h"
#include "game.h"
#include <QDebug>

RampBuilderLemming::RampBuilderLemming()
{
    this -> setPixmap(QPixmap(":/graphics/RampBuilderLemming"));
}

void RampBuilderLemming::move()
{
    QList<QGraphicsItem*> colliding_items = this -> collidingItems();

    if(colliding_items.size() != 0)
        resolve_collision(colliding_items);

    if(lemming_is_waiting_for_deletion)
        return;

    int old_y_axis_speed = y_axis_speed;
    update_axis_speeds();

    if(building_started && (this -> pos().x() >= last_block_built_pos_x - default_x_axis_speed && this -> pos().x() <= last_block_built_pos_x + default_x_axis_speed))
        check_for_farther_building_instructions();

    if(old_y_axis_speed != y_axis_speed && fall_height_counter >= death_fall_height)
    {
        safely_delete_lemming(false);
        return;
    }

    this -> setPos(pos().x() + x_axis_speed, pos().y()+ y_axis_speed);

    bool lemming_out_of_scene = check_for_lemming_out_of_scene();
    if(lemming_out_of_scene)
        safely_delete_lemming(false);

    largest_obstacle_height_prevalence = -1;
}

void RampBuilderLemming::resolve_collision(QList<QGraphicsItem*> colliding_items)
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

void RampBuilderLemming::bounce_back()
{
    x_axis_speed = -x_axis_speed;
    this -> setPixmap(this->pixmap().transformed(QTransform().scale(-1, 1)));
}

void RampBuilderLemming::check_for_farther_building_instructions()
{
    if(blocks_to_build > 0)
    {
        build();
        y_axis_speed = 0;
    }

    if(blocks_to_build == 0)
        fire_lemming();
}

void RampBuilderLemming::resolve_collision_with_block(QGraphicsItem *block)
{
    int height_difference = this -> pos().y() + boundingRect().height() - block -> pos().y();
    bool lemming_is_moving_in_direction_of_the_obstacle = check_for_lemming_moving_in_direction_of_the_obstacle(block);

    if(height_difference > this -> boundingRect().height()/2 && lemming_is_moving_in_direction_of_the_obstacle)
        bounce_back();

    else if(height_difference > largest_obstacle_height_prevalence)
        largest_obstacle_height_prevalence = height_difference;

    if(building_started == false)
    {
        build();
        building_started = true;
    }
}

void RampBuilderLemming::resolve_collision_with_blocker(QGraphicsItem *blocker)
{
    bool lemming_is_moving_in_direction_of_the_blocker = check_for_lemming_moving_in_direction_of_the_obstacle(blocker);
    if(lemming_is_moving_in_direction_of_the_blocker)
        bounce_back();
}

void RampBuilderLemming::update_axis_speeds()
{
    if(largest_obstacle_height_prevalence == -1)
    {
        y_axis_speed = default_y_axis_speed;
        fall_height_counter += y_axis_speed;
    }
    else
    {
        y_axis_speed = 0;

        if(fall_height_counter < death_fall_height)
            fall_height_counter = 0;

        if(largest_obstacle_height_prevalence <= this -> boundingRect().height()/2)
            y_axis_speed = -largest_obstacle_height_prevalence;
    }
}

void RampBuilderLemming::build()
{
    int new_block_pos_x;
    int x_axis_versor = x_axis_speed/default_x_axis_speed;

    if(blocks_to_build == 3)
        new_block_pos_x = this -> pos().x() + this -> boundingRect().width() * x_axis_versor;
    else
        new_block_pos_x = last_block_built_pos_x + block_side_length * x_axis_versor;
    int new_block_pos_y = ceil(this -> pos().y() + this -> boundingRect().height()/2);
    last_block_built_pos_x = new_block_pos_x;

    Game::Get().get_map()->add_block(new_block_pos_x, new_block_pos_y);

    blocks_to_build --;
}
