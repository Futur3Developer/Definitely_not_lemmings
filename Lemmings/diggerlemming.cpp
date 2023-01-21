#include "diggerlemming.h"
#include "blockerlemming.h"
#include "MapFiles/MapComponents/exit.h"
#include "game.h"
#include <QRandomGenerator>
#include <QDebug>

DiggerLemming::DiggerLemming()
{
    this -> setPixmap(QPixmap(":/graphics/DiggerLemmingPassive"));
}

void DiggerLemming::move()
{
    if(block_layers_to_digg_out == 0)
        fire_lemming();

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

    int x_axis_shaking_offset = 0;
    if(digging_mode == 1)
        x_axis_shaking_offset = QRandomGenerator::global() -> bounded(4) - 2;

    this -> setPos(pos().x() + x_axis_speed + x_axis_shaking_offset, pos().y() + y_axis_speed);

    bool lemming_out_of_scene = check_for_lemming_out_of_scene();
    if(lemming_out_of_scene)
        safely_delete_lemming(false);

    largest_obstacle_height_prevalence = -1;
    block_layer_was_digged_out = false;
}

void DiggerLemming::resolve_collision(QList<QGraphicsItem*> colliding_items)
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

void DiggerLemming::bounce_back()
{
    x_axis_speed = -x_axis_speed;
    this -> setPixmap(this->pixmap().transformed(QTransform().scale(-1, 1)));
}

void DiggerLemming::start_digging()
{
    digging_mode = 1;
    this -> setPixmap(QPixmap(":/graphics/DiggerLemmingDigging"));

    if(x_axis_speed < 0)
        this -> setPixmap(this -> pixmap().transformed(QTransform().scale(-1, 1)));

    this -> x_axis_speed = 0;
}

void DiggerLemming::digg_out_blocks_layer(QGraphicsItem *block)
{
    Map *map = Game::Get().get_map();

    QGraphicsItem *block_on_the_left = map->itemAt(block->x() - 40, block->y() + 40, QTransform());
    QGraphicsItem *block_on_the_right = map->itemAt(block->x() + 120, block->y() + 40, QTransform());
    QGraphicsItem *block_on_the_block = map->itemAt(block->x() + 40, block->y() + 40, QTransform());

    while((block_on_the_left != nullptr && block_on_the_left -> type() == Block::Type) || (block_on_the_right != nullptr && block_on_the_right -> type() == Block::Type) || (block_on_the_block != nullptr && block_on_the_block -> type() == Block::Type))
    {
        if(block_on_the_left != nullptr && block_on_the_left -> type() == Block::Type)
            block_on_the_left -> hide();

        if(block_on_the_right != nullptr && block_on_the_right -> type() == Block::Type)
            block_on_the_right -> hide();

        if(block_on_the_block != nullptr && block_on_the_block -> type() == Block::Type)
            block_on_the_block -> hide();

        block_on_the_left = map->itemAt(block->x() - 40, block->y() + 40, QTransform());
        block_on_the_right = map->itemAt(block->x() + 120, block->y() + 40, QTransform());
        block_on_the_block = map->itemAt(block->x() + 40, block->y() + 40, QTransform());
    }

    if(block_layer_was_digged_out == false)
        block_layers_to_digg_out --;

    block_layer_was_digged_out = true;
}

void DiggerLemming::resolve_collision_with_block(QGraphicsItem *block)
{
    double height_difference = this -> pos().y() + boundingRect().height() - block -> pos().y();
    bool lemming_is_moving_in_direction_of_the_obstacle = check_for_lemming_moving_in_direction_of_the_obstacle(block);

    if(height_difference > this -> boundingRect().height()/2 && lemming_is_moving_in_direction_of_the_obstacle)
        bounce_back();
    else if(height_difference > largest_obstacle_height_prevalence)
        largest_obstacle_height_prevalence = height_difference;

    if(height_difference == 0)
    {
        if(digging_mode == 0)
            start_digging();

        if(block_layers_to_digg_out > 0)
            digg_out_blocks_layer(block);
    }
}

void DiggerLemming::resolve_collision_with_blocker(QGraphicsItem *blocker)
{
    bool lemming_is_moving_in_direction_of_the_blocker = check_for_lemming_moving_in_direction_of_the_obstacle(blocker);
    if(lemming_is_moving_in_direction_of_the_blocker)
        bounce_back();
}

void DiggerLemming::update_axis_speeds()
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

