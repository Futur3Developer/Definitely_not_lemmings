#ifndef MAPCREATOR_H
#define MAPCREATOR_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>

#include "MapFiles/MapComponents/block.h"
#include "MapComponents/exit.h"
#include "MapComponents/entrance.h"
#include "map.h"

/**
 * Class allowing user to design and save Map instance.
 */
class MapCreator: public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * MapCreator constructor.
     * @param map - pointer to Map instance. If already exisiting Map is to be edited pointer to it should be passed.
     * Otherwise new Map instance will be created.
     */
    MapCreator(Map *map);
    ~MapCreator();
    Map *get_map();

private:
    Map *map;

    void set_view_configuration();

    /**
     * Initializes map components to be placed - Block, Entrance and Exit. Places them on map's gui panel.
     * @param map_is_being_edited - if true, entrance and exit are not placed.
     */
    void initialize_map_components(bool map_is_being_edited);


    /**
     * Adds map creation guideline as QGraphicsTextItem to the Map instance.
     */
    void add_guideline(QString text, int x_pos, int y_pos);
    void add_map_creation_guidelines();
    void add_button(QPushButton *button, int aleft, const QString &text);
    void add_buttons();

    void mousePressEvent(QMouseEvent *event);
    /**
     * Picks up map component selected by user - sets attributes so that component will be dragged by the mouse.
     * @param component - component to be dragged by the mouse cursor.
     */
    void pick_up_component(QGraphicsItem *component);
    /**
     * Changes position of component_to_place to position of @param:event if component_is_being_placed is true.
     */
    void mouseMoveEvent(QMouseEvent *event);

    /**
     * Restores component to position it was at when @fn:pick_up_component() was called.
     */
    void restore_component_to_latest_position();

    /**
     * Places component_to_place at @param:new_position if it is deemed valid. Otherwise component is still being dragged by the cursor.
     */
    void place_component(QPointF new_position);

    /**
     * Checks if new_position of component_to_place is valid - is within boundaries of Map and does not overlap with gui panel.
     * @param new_position - position which validity is being checked, presumably new position of component_to_place
     * @return - returns true if component can be placed at new_position according to given restrictions. Otherwise returns false.
     */
    bool assert_that_position_is_valid(QPointF new_position);

    /**
     * Checks if there is Block instance at block_original_position.
     * @return - returns true if there is Block instance. Otherwise returns false.
     */
    bool check_if_there_is_new_block_to_place();

    /**
     * Removes component from Map instance. In reality it only removes and deletes Block instances.
     * Entrance or Exit positions are always set to their respective original_position.
     */
    void remove_component_from_map();

    /**
     * Prepares new Map instance based on the one that is currently editet by MapCreator.
     * Adds to it all the items that are not overlapping with Map's gui panel.
     * Attemps to get lemmings configuration from user by calling @fn:fill_map_lemmings_parameters.
     * @return - if lemmings configuration is successfully set returns pointer to new Map instance.
     *  Otherwise returns nullptr to be handled by the calling function.
     */
    Map *prepare_map_without_gui();

    /**
     * Checks if instances of Exit and Entrance are still on their respective original_position's.
     * @return - returns true if Exit and Entrance are not on their respective original_position's. Otherwise returns false.
     */
    bool assert_that_required_components_are_placed();

    /**
     * Attemps to get lemmings configuration (available lemmings class changes and lemmings survival rate) from user and set them in @param:map.
     * Getting those values is done through QDialog instance filled with QLineEdits validated for int values.
     * If dialog is accepted by the user and all values were provided @fn:set_map_lemmings_parameters is called.
     * @param map - pointer to instance of Map that is to be filled with provided values.
     * If it already has lemmings configuration set, respective values are shown in QLineEdits.
     */
    void fill_map_lemmings_parameters(Map *map);
    void prepare_dialog_buttons(QFormLayout *form, QDialog *dialog);
    void add_dialog_line_edit_int_validated(QDialog *dialog, QList<QLineEdit*> *line_edits_list, QString label, QFormLayout *form,
                                            int validator_limit, int parameter_index);
    void set_map_lemmings_parameters(QList<QLineEdit *> parameters_to_fill_line_edits, Map *map);

    QPointF entrance_original_position;
    QPointF exit_original_position;
    QPointF block_original_position;
    QPointF component_original_position;

    bool component_is_being_placed = false;
    QGraphicsItem *component_to_place;

    QPushButton *back_button;
    QPushButton *save_map_button;
    QPushButton *play_button;
    bool lemmings_configuration_is_set = false;

private slots:
    /**
     * Shows MainWindow to the user and closes MapCreator instance.
     */
    void back_to_main_menu();

    /**
     * Checks if required components of Map are placed and attemps to get lemmings configuration through @fn:fill_map_lemmings_parameters.
     * If any of those conditions is not fullfilled it will return and show user QMessageBox::warning with corresponding warning.
     * Otherwise it will call MapConversionManager's method process_map_saving_request in attempt to save Map instance currently edited by MapCreator.
     */
    void request_map_saving();

    /**
     * Checks if required components of Map are placed and calls @fn:prepare_map_without_gui.
     * If returned pointer is valid calls Game's method initialize_game and closes MapCreator instance.
     * If any of required conditions is not fullfilled it will return and show user QMessageBox:warning with corresponding warning.
     */
    void start_game();
};
#endif // MAPCREATOR_H
