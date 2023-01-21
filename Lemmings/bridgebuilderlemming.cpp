#include "bridgebuilderlemming.h"
#include "blockerlemming.h"
#include "MapFiles/MapComponents/exit.h"
#include "game.h"
#include <QDebug>

BridgeBuilderLemming::BridgeBuilderLemming()
{
    this -> setPixmap(QPixmap(":/graphics/BridgeBuilderLemming"));
}

void BridgeBuilderLemming::move()
{
    QList<QGraphicsItem*> colliding_items = this -> collidingItems();

    if(colliding_items.size() != 0)
        resolve_collision(colliding_items);

    if(lemming_is_waiting_for_deletion)
        return;

    int old_y_axis_speed = y_axis_speed;
    update_axis_speeds();

    if(y_axis_speed - old_y_axis_speed > 0)
        respond_to_end_of_ground();

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

void BridgeBuilderLemming::resolve_collision(QList<QGraphicsItem*> colliding_items)
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

void BridgeBuilderLemming::bounce_back()
{
    x_axis_speed = -x_axis_speed;
    this -> setPixmap(this->pixmap().transformed(QTransform().scale(-1, 1)));
}

void BridgeBuilderLemming::resolve_collision_with_block(QGraphicsItem *block)
{
    double height_difference = this -> pos().y() + boundingRect().height() - block -> pos().y();
    bool lemming_is_moving_in_direction_of_the_obstacle = check_for_lemming_moving_in_direction_of_the_obstacle(block);

    if(height_difference > this -> boundingRect().height()/2 && lemming_is_moving_in_direction_of_the_obstacle)
        bounce_back();
    else if(height_difference > largest_obstacle_height_prevalence)
        largest_obstacle_height_prevalence = height_difference;
}

void BridgeBuilderLemming::resolve_collision_with_blocker(QGraphicsItem *blocker)
{
    bool lemming_is_moving_in_direction_of_the_blocker = check_for_lemming_moving_in_direction_of_the_obstacle(blocker);
    if(lemming_is_moving_in_direction_of_the_blocker)
        bounce_back();
}

void BridgeBuilderLemming::update_axis_speeds()
{
    if(largest_obstacle_height_prevalence == -1)
    {
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

void BridgeBuilderLemming::build()
{
    int x_axis_versor = x_axis_speed/default_x_axis_speed;
    //Offset is needed when going right because boundingRect != outline used for collision detection
    int block_pos_x_offset = (x_axis_versor + 1)/2 * boundingRect().width()/6;
    int new_block_pos_x = this -> pos().x() + block_pos_x_offset;
    int new_block_pos_y = this -> pos().y() + this -> boundingRect().height();

    Game::Get().get_map()->add_block(new_block_pos_x, new_block_pos_y);

    blocks_to_build --;
}

void BridgeBuilderLemming::respond_to_end_of_ground()
{
    if(blocks_to_build > 0)
    {
        build();
        y_axis_speed = 0;
    }

    if(blocks_to_build == 0)
        fire_lemming();
}
