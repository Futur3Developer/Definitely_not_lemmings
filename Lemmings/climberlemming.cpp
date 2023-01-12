#include "climberlemming.h"
#include "blockerlemming.h"
#include "joblesslemming.h"
#include "MapFiles/MapComponents/exit.h"
#include "game.h"


ClimberLemming::ClimberLemming()
{
    this -> setPixmap(QPixmap(":/graphics/ClimberLemming"));
}

void ClimberLemming::move()
{
    QList<QGraphicsItem*> colliding_items = this -> collidingItems();

    if(colliding_items.size() != 0)
    {
        resolve_collision(colliding_items);
        if(lemming_is_waiting_for_deletion)
            return;
    }
    update_axis_speeds();

    if(climbing_mode == 1 && fall_height_counter >= this->boundingRect().height())
        stop_climbing();

    this -> setPos(pos().x() + x_axis_speed, pos().y()+ y_axis_speed);

    bool lemming_out_of_scene = check_for_lemming_out_of_scene();
    if(lemming_out_of_scene)
        safely_delete_lemming(false);

    largest_obstacle_height_prevalence = -1;
}

void ClimberLemming::resolve_collision(QList<QGraphicsItem*> colliding_items)
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

void ClimberLemming::bounce_back()
{
    x_axis_speed = -x_axis_speed;
    this -> setPixmap(this->pixmap().transformed(QTransform().scale(-1, 1)));
}

void ClimberLemming::start_climbing()
{
    climbing_mode = 1;
    x_axis_versor_before_climbing_started = x_axis_speed/default_x_axis_speed;
    y_axis_speed = -default_y_axis_speed;
    x_axis_speed = 0;
    if(x_axis_versor_before_climbing_started < 0)
        this -> setPixmap(this->pixmap().transformed(QTransform().scale(-1, 1)));

    this -> setPixmap(this->pixmap().transformed(QTransform().rotate((x_axis_versor_before_climbing_started)*(-90))));
    this -> setPos(pos().x(), pos().y() - (this->boundingRect().height()/2));
}

void ClimberLemming::stop_climbing()
{
    x_axis_speed = x_axis_versor_before_climbing_started*default_x_axis_speed;
    this -> setPixmap(this->pixmap().transformed(QTransform().rotate((x_axis_versor_before_climbing_started)*90)));
    fire_lemming();
}

void ClimberLemming::resolve_collision_with_block(QGraphicsItem *block)
{
    double height_difference_for_vertical_plane_movement = x_axis_versor_before_climbing_started*(this -> pos().x() + boundingRect().width()*x_axis_versor_before_climbing_started - block -> pos().x())*climbing_mode;
    double height_difference_for_horizontal_plane_movement;

    if(climbing_mode == 0)
        height_difference_for_horizontal_plane_movement = this -> pos().y() + boundingRect().height() - block -> pos().y();
    else
        height_difference_for_horizontal_plane_movement = 0;

    double height_difference = height_difference_for_vertical_plane_movement + height_difference_for_horizontal_plane_movement;
    bool lemming_is_moving_in_direction_of_the_obstacle = check_for_lemming_moving_in_direction_of_the_obstacle(block);

    if(height_difference > this -> boundingRect().height()/2 && lemming_is_moving_in_direction_of_the_obstacle)
    {
        if(climbing_mode == 0)
            start_climbing();
        else
        {
            this -> setPos(this -> pos().x() + this -> boundingRect().width()*(-(x_axis_versor_before_climbing_started))/2, this -> pos().y() + 10);
            stop_climbing();
        }
    }
    else if(height_difference > largest_obstacle_height_prevalence)
        largest_obstacle_height_prevalence = height_difference;
}

void ClimberLemming::resolve_collision_with_blocker(QGraphicsItem *blocker)
{
    bool lemming_is_moving_in_direction_of_the_blocker = check_for_lemming_moving_in_direction_of_the_obstacle(blocker);
    if(lemming_is_moving_in_direction_of_the_blocker)
        bounce_back();
}

void ClimberLemming::update_axis_speeds()
{
    if(largest_obstacle_height_prevalence == -1)
        if(climbing_mode == 0)
            y_axis_speed = default_y_axis_speed;
        else
        {
            x_axis_speed = x_axis_versor_before_climbing_started*default_x_axis_speed;
            fall_height_counter += abs(x_axis_speed);
        }
    else
    {
        if(climbing_mode == 0)
            y_axis_speed = 0;
        else
            x_axis_speed = 0;
        fall_height_counter = 0;

        if(largest_obstacle_height_prevalence <= this -> boundingRect().height()/2)
        {
            if(climbing_mode == 0)
                y_axis_speed = -largest_obstacle_height_prevalence;
            else
                x_axis_speed = -(x_axis_versor_before_climbing_started)*largest_obstacle_height_prevalence;
        }
    }
}

bool ClimberLemming::check_for_lemming_moving_in_direction_of_the_obstacle(QGraphicsItem *obstacle)
{
    int position_difference;
    if(climbing_mode == 0)
    {
        position_difference = obstacle -> pos().x() - this -> pos().x();
        if(check_for_same_sign(position_difference, x_axis_speed))
            return true;
    }
    else
    {
        position_difference = obstacle -> pos().y() - this -> pos().y();
        if(check_for_same_sign(position_difference, y_axis_speed))
            return true;
    }
    return false;
}
