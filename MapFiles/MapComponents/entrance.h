#ifndef ENTRANCE_H
#define ENTRANCE_H

#include <QGraphicsItem>
#include <QBrush>
#include <QKeyEvent>


/**
 * Spawning point for lemmings. Required component of Map instance to start game.
 */
class Entrance: public QGraphicsRectItem
{
public:
    Entrance();
    ~Entrance();

    enum { Type = UserType + 2 };

    int type() const override { return Type; }

    /**
     * Adds new instance of JoblessLemming to the Map that is currently used by Game instance at position of Entrance.
     * Memory of JoblessLemming is managed by Map.
     * @param delay_should_be_applied - if true 1500ms of delay before spawning lemming are applied. Otherwise no delay is applied.
     */
    void spawn_lemming(bool delay_should_be_applied);
    void delay_spawn(int delay_time_in_milliseconds);

private:

};

#endif // ENTRANCE_H
