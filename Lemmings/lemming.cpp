#include "lemming.h"
#include "paratrooperlemming.h"
#include "joblesslemming.h"
#include "blockerlemming.h"
#include "basherlemming.h"
#include "diggerlemming.h"
#include "climberlemming.h"
#include "bridgebuilderlemming.h"
#include "rampbuilderlemming.h"
#include "game.h"

#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>
#include <QImage>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

Lemming::Lemming()
{
    setPixmap(QPixmap(":/graphics/lemming_right"));
    this -> setOpacity(0.8);
    this -> setFlag(ItemIsFocusable, true);
    this -> setZValue(-1);

    lower_lemmings_death_boundary = Game::Get().get_map() -> get_gui_panel_boundary_y_pos();
    higher_lemmings_death_boundary = -(this -> boundingRect().height());
    left_lemmings_death_boundary = -(this -> boundingRect().width());
    right_lemmings_death_boundary = Game::Get().get_map() -> get_width() + this -> boundingRect().width();
}

void Lemming::move(){}

void Lemming::keyPressEvent(QKeyEvent *event)
{
    //Catch only keys from 1 to 7 used to change lemming's classes
    int event_key = event -> key();
    if(event_key < Qt::Key_1 || event_key > Qt::Key_7)
        return;

    int current_available_class_changes_amount = Game::Get().get_available_lemmings_class_changes(event_key - Qt::Key_1);

    if(accept_class_change == false || current_available_class_changes_amount == 0)
        return;

    if(event_key == Qt::Key_1)
    {
        Lemming *lemming = new BlockerLemming;
        change_class(lemming);
    }
    else if(event_key == Qt::Key_2)
    {
        Lemming *lemming = new ClimberLemming;
        change_class(lemming);
    }
    else if(event_key == Qt::Key_3)
    {
        Lemming *lemming = new ParatrooperLemming;
        change_class(lemming);
    }
    else if(event_key == Qt::Key_4)
    {
        Lemming *lemming = new BasherLemming;
        change_class(lemming);
    }
    else if(event_key == Qt::Key_5)
    {
        Lemming *lemming = new DiggerLemming;
        change_class(lemming);
    }
    else if(event_key == Qt::Key_6)
    {
        Lemming *lemming = new BridgeBuilderLemming;
        change_class(lemming);
    }
    else if(event_key == Qt::Key_7)
    {
        Lemming *lemming = new RampBuilderLemming;
        change_class(lemming);
    }

    QPushButton *button_corresponding_to_performed_class_change = Game::Get().class_changing_buttons[event_key - Qt::Key_1];
    button_corresponding_to_performed_class_change -> animateClick();
    Game::Get().decrease_available_lemmings_class_changes(event_key - Qt::Key_1);

    int new_available_class_changes_amount = current_available_class_changes_amount - 1;

    if(new_available_class_changes_amount == 0)
        button_corresponding_to_performed_class_change -> setEnabled(false);

    Game::Get().set_focused_lemming(nullptr);
}

void Lemming::change_class(Lemming *lemming)
{
    this -> hide();
    Game::Get().get_map()->removeItem(this);
    lemming -> setPos(this -> pos());
    lemming -> fall_height_counter = this -> fall_height_counter;
    lemming -> y_axis_speed = this -> y_axis_speed;

    if(lemming -> boundingRect() != this -> boundingRect())
        adjust_position_for_lemmings_height_difference(lemming);

    Game::Get().get_map()->addItem(lemming);
    Game::Get().update_lemmings_alive_list(this, lemming);
    lemming -> setOpacity(1);
    lemming -> setFlag(ItemIsFocusable, false);

    if(this -> x_axis_speed < 0)
    {
        lemming -> setPixmap(lemming -> pixmap().transformed(QTransform().scale(-1, 1)));
        lemming -> x_axis_speed = -(lemming -> x_axis_speed);
    }

    this -> deleteLater();
}

void Lemming::fire_lemming()
{
    Lemming *lemming = new JoblessLemming;
    change_class(lemming);
    lemming -> setOpacity(0.8);
    lemming -> setFlag(ItemIsFocusable, true);
}

void Lemming::adjust_position_for_lemmings_height_difference(Lemming *lemming)
{
    int height_difference = this -> boundingRect().height() - lemming -> boundingRect().height();
    lemming -> setPos(lemming -> pos().x(), lemming -> pos().y() + height_difference);
}

void Lemming::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!(event -> button() == Qt::LeftButton) || accept_class_change == false)
        return;

    Lemming *focused_lemming = Game::Get().get_focused_lemming();
    if(focused_lemming != nullptr)
    {
        focused_lemming -> clearFocus();
        focused_lemming -> setOpacity(0.5);
    }

    Game::Get().set_focused_lemming(this);
    this -> setFocus();
    this -> setOpacity(1);
}

bool Lemming::check_for_lemming_out_of_scene()
{
    //It may be confusing but remember that scenes position (0, 0) is its left corner
    if (pos().y() < higher_lemmings_death_boundary || pos().y() > lower_lemmings_death_boundary || pos().x() < left_lemmings_death_boundary || pos().x() > right_lemmings_death_boundary)
        return true;
    return false;
}

void Lemming::resolve_collision_with_exit()
{
    Game::Get().get_score_pointer() -> increase_saved_lemmings_counter();
    lemming_is_waiting_for_deletion = true;
    safely_delete_lemming(true);
}

void Lemming::safely_delete_lemming(bool lemming_was_saved)
{
    if(!lemming_was_saved)
        Game::Get().get_score_pointer()->increase_dead_lemmings_counter();

    this -> hide();
    this -> setFlag(ItemIsFocusable, false);
    Game::Get().update_lemmings_alive_list(this, nullptr);
    this -> deleteLater();
}

bool Lemming::check_for_same_sign(int num1, int num2)
{
  return (num1 ^ num2) >= 0;
}

bool Lemming::check_for_lemming_moving_in_direction_of_the_obstacle(QGraphicsItem *obstacle)
{
    int horizontal_position_difference = obstacle -> pos().x() - this -> pos().x();
    return check_for_same_sign(horizontal_position_difference, this -> x_axis_speed);
}
