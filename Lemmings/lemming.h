#ifndef LEMMING_H
#define LEMMING_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Lemming : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Lemming();
    virtual ~Lemming() = default;
    virtual void move();

    //Mouse events and key press events related methods
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    //Methods for changing class of focused lemming
    void change_class(Lemming* lemming);
    void fire_lemming();

    //Methods evoked in move()
    bool check_for_lemming_out_of_scene();
    bool check_for_lemming_moving_in_direction_of_the_obstacle(QGraphicsItem *obstacle);
    void resolve_collision_with_exit(); 
    bool check_for_same_sign(int num1, int num2);
    void safely_delete_lemming(bool lemming_was_saved);

    //Basic lemming's attributes
    int x_axis_speed = 2;
    int y_axis_speed = 0;
    const int default_x_axis_speed = 2;
    const int default_y_axis_speed = 2;
    int largest_obstacle_height_prevalence = -1;
    bool accept_class_change = false;
    bool lemming_is_waiting_for_deletion = false;
    int fall_height_counter = 0;
    int death_fall_height = 320;

private:
    //Attributes used to simplify check_fo_lemming_out_of_scene() method
    int lower_lemmings_death_boundary;
    int higher_lemmings_death_boundary;
    int left_lemmings_death_boundary;
    int right_lemmings_death_boundary;

    //Method used in case of changing lemming's class to one with graphic of different height (Paratrooper lemming)
    void adjust_position_for_lemmings_height_difference(Lemming *lemming);
};

#endif // LEMMING_H
