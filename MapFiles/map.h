#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>

#include "Lemmings/lemming.h"
#include "MapFiles/MapComponents/block.h"

/**
 * Level would be propably better class name. Even better, make a new class Level with lemmings configuration and Map as one of attributes.
 */
class Map : public QGraphicsScene
{
public:
    Map();
    virtual ~Map() = default;

    /**
     * @brief Adds new instance of Block to Map.
     * @param x_pos - block's x position
     * @param y_pos - block's y position
     */
    void add_block(int x_pos, int y_pos);

    /**
     * @brief Adds new instance of Entrance to Map.
     * @param x_pos - entrance's x position
     * @param y_pos - entrance's y position
     */
    void add_entrance(int x_pos, int y_pos);

    /**
     * @brief Adds new instance of Exit to Map.
     * @param x_pos - exit's x position
     * @param y_pos - exit's y position
     */
    void add_exit(int x_pos, int y_pos);

    /**
     * @brief Adds new instance of QGraphicsRectItem to Map at position (0, @param:panel_y_pos).
     * It's width is equal to width of Map and it's height is @param:panel_height.
     */
    void add_gui_panel(int panel_height, int panel_y_pos);

    int get_width() const;
    int get_height() const;
    int get_gui_panel_boundary_y_pos() const;

    /**
     * List of amounts of available class changes to Lemming's child classes corresponding to list index.
     */
    QList<int> available_lemmings_class_changes;

    /**
     * How many lemmings are to be spawned from Entrance.
     */
    int lemmings_to_spawn;

    /**
     * How many lemmings are to be saved to win the game.
     */
    int lemmings_to_save;

private:
    const int map_width = 1920;
    const int map_height = 960;
    const int gui_panel_boundary_y_pos = 750;
};

#endif // MAP_H
