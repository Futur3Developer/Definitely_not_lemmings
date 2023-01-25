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

class MapCreator: public QGraphicsView
{
    Q_OBJECT
public:
    MapCreator(Map *map);
    Map *get_map();

private:
    Map *map;

    ///Basic initialization methods
    void set_view_configuration();
    void initialize_map_components(bool map_is_being_edited);
    //GUI initialization methods
    void add_guideline(QString text, int x_pos, int y_pos);
    void add_map_creation_guidelines();
    void add_button(QPushButton *button, int aleft, const QString &text);
    void add_buttons();

    //Mouse events related methods
    void mousePressEvent(QMouseEvent *event);
    void pick_up_component(QGraphicsItem *component);
    void mouseMoveEvent(QMouseEvent *event);

    //Components placing and removing methods
    void restore_component_to_latest_position();
    void place_component(QPointF new_position);
    bool assert_that_position_is_valid(QPointF new_position);
    bool check_if_there_is_new_block_to_place();
    void remove_component_from_map();

    ///Methods for preparing valid map/level for game
    Map *prepare_map_without_gui();
    bool assert_that_required_components_are_placed();
    //Methods for getting map lemmings parameters from user and setting them
    void fill_map_lemmings_parameters(Map *map);
    void add_dialog_line_edit_int_validated(QDialog *dialog, QList<QLineEdit*> *line_edits_list, QString label, QFormLayout *form,
                                            int validator_limit, int parameter_index);
    void set_map_lemmings_parameters(QList<QLineEdit *> parameters_to_fill_line_edits, Map *map);

    //Attributes for placing GUI and removing components to correct positions
    QPointF entrance_original_position;
    QPointF exit_original_position;
    QPointF block_original_position;
    QPointF component_original_position;

    //Attributes for component placing
    bool component_is_being_placed = false;
    QGraphicsItem *component_to_place;

    //GUI attributes
    QPushButton *back_button;
    QPushButton *save_map_button;
    QPushButton *play_button;
    bool lemmings_configuration_is_set = false;

    void prepare_dialog_buttons(QFormLayout *form, QDialog *dialog);

private slots:
    void back_to_main_menu();
    void request_map_saving();
    void start_game();
};
#endif // MAPCREATOR_H
