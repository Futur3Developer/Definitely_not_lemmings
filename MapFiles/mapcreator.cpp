#include "mapcreator.h"
#include "MapConversion/mapconversionmanager.h"
#include "mainwindow.h"
#include "Lemmings/blockerlemming.h"

#include <QLabel>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QDebug>

///TODO:: Add saving and loading map lemmings parameters

MapCreator::MapCreator(Map *map)
{
    bool map_is_being_edited = true;
    if(map == nullptr)
    {
        map = new Map();
        map_is_being_edited = false;
    }

    this -> setAttribute(Qt::WA_DeleteOnClose);
    this -> map = map;

    map -> add_gui_panel(map -> get_height() - map -> get_gui_panel_boundary_y_pos(), map -> get_gui_panel_boundary_y_pos());
    set_view_configuration();
    initialize_map_components(map_is_being_edited);
    add_map_creation_guidelines();
    add_buttons();
}

Map *MapCreator::get_map()
{
    return map;
}

void MapCreator::set_view_configuration()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(map -> get_width(), map -> get_height());
    setScene(map);
}

void MapCreator::initialize_map_components(bool map_is_being_edited)
{
    block_original_position.setX(map -> get_width()/2 - 40);
    block_original_position.setY(map -> get_height() - 120);

    entrance_original_position.setX(block_original_position.x() - 300);
    entrance_original_position.setY(block_original_position.y() - 20);

    exit_original_position.setX(block_original_position.x() + 300);
    exit_original_position.setY(block_original_position.y() - 20);

    map -> add_block(block_original_position.x(), block_original_position.y());

    if(map_is_being_edited)
        return;

    map -> add_entrance(entrance_original_position.x(), entrance_original_position.y());
    map -> add_exit(exit_original_position.x(), exit_original_position.y());
}

void MapCreator::add_guideline(QString text, int x_pos, int y_pos)
{
    QGraphicsTextItem *temp = new QGraphicsTextItem();

    temp -> setPlainText(text);
    temp -> setDefaultTextColor(Qt::black);
    temp -> setFont(QFont("times",16));
    temp -> setPos(x_pos, y_pos);

    map -> addItem(temp);
}
void MapCreator::add_map_creation_guidelines()
{
    add_guideline("Components to place:", entrance_original_position.x() - 300, entrance_original_position.y() + 40);

    int text_height = entrance_original_position.y() - 45;

    add_guideline("Entrance", entrance_original_position.x() - 10, text_height);
    add_guideline("Blocks", block_original_position.x(), text_height);
    add_guideline("Exit", exit_original_position.x() + 15, text_height);
}

void MapCreator::add_button(QPushButton *button, int aleft, const QString &text)
{
    button -> setGeometry(QRect(aleft, entrance_original_position.y() + 30, 100, 50));
    button -> setText(text);

    map -> addWidget(button);
}

void MapCreator::add_buttons()
{
    QPushButton *back_button = new QPushButton();
    add_button(back_button, 50, "Back");
    connect(back_button, &QPushButton::clicked, this, &MapCreator::back_to_main_menu);

    QPushButton *play_button = new QPushButton();
    add_button(play_button, map -> get_width() - 150, "Play");
    connect(play_button, &QPushButton::clicked, this, &MapCreator::start_game);

    QPushButton *save_map_button = new QPushButton();
    add_button(save_map_button, map -> get_width() - 270, "Save map");
    connect(save_map_button, &QPushButton::clicked, this, &MapCreator::request_map_saving);
}

void MapCreator::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *clicked_item = itemAt(event ->pos());

    if((event ->buttons() & Qt::LeftButton) && !(clicked_item == nullptr) && component_is_being_placed == false)
    {
        if(clicked_item -> type() == Block::Type || clicked_item -> type() == Entrance::Type || clicked_item -> type() == Exit::Type)
        {
            pick_up_component(clicked_item);
        }
    }
    else if (component_is_being_placed == true)
    {
        if(event -> buttons() & Qt::RightButton)
        {
            restore_component_to_latest_position();
        }
        else if(event -> buttons() & Qt::LeftButton)
        {
            place_component(event -> pos());
        }
        else if(event -> buttons() & Qt::MiddleButton)
        {
           remove_component_from_map();
        }
    }

    QGraphicsView::mousePressEvent(event);
}


void MapCreator::pick_up_component(QGraphicsItem *component)
{
    component_original_position = component -> pos();
    component_is_being_placed = true;
    component_to_place = component;
}

void MapCreator::mouseMoveEvent(QMouseEvent *event)
{
    if(component_is_being_placed == true)
    {
        component_to_place -> setPos(event->pos().x(), event->pos().y());
    }

    QGraphicsView::mouseMoveEvent(event);
}

void MapCreator::restore_component_to_latest_position()
{
    component_is_being_placed = false;
    component_to_place->setPos(component_original_position);
}


void MapCreator::place_component(QPointF new_position)
{
    bool position_is_valid = assert_that_position_is_valid(new_position);

    if(!(position_is_valid))
        return;

    component_is_being_placed = false;
    component_to_place -> setPos(new_position);

    bool there_is_new_block_to_place = check_if_there_is_new_block_to_place();

    if(!(there_is_new_block_to_place))
        this -> map ->add_block(block_original_position.x(), block_original_position.y());

}

bool MapCreator::assert_that_position_is_valid(QPointF new_position)
{
    if(component_to_place -> type() == Block::Type)
    {
        if(new_position.y() < map -> get_gui_panel_boundary_y_pos() - 80 && new_position.x() < map -> get_width() - 80)
            return true;
        return false;
    }
    else if (new_position.y() < map -> get_gui_panel_boundary_y_pos() - 120 && new_position.x() < map -> get_width() - 90)
        return true;
    return false;
}

bool MapCreator::check_if_there_is_new_block_to_place()
{
    if(map -> itemAt(block_original_position, QTransform()) -> type() != Block::Type)
    {
        return false;
    }

    return true;
}

void MapCreator::remove_component_from_map()
{
    component_is_being_placed = false;
    component_to_place -> setZValue(1);

    if(component_to_place -> type() == Entrance::Type)
        component_to_place -> setPos(entrance_original_position);
    else if (component_to_place -> type() == Exit::Type)
        component_to_place -> setPos(exit_original_position);
    else //It can only be a block
    {
        bool there_is_new_block_to_place = check_if_there_is_new_block_to_place();
        if(there_is_new_block_to_place)
        {
            map -> removeItem(component_to_place);
            delete component_to_place;
        }
        else
            component_to_place -> setPos(block_original_position);
    }
}

bool MapCreator::assert_that_required_components_are_placed()
{
    if((map -> itemAt(entrance_original_position, QTransform())->type() == Entrance::Type) || (map -> itemAt(exit_original_position, QTransform())->type() == Exit::Type))
    {
        return false;
    }
    return true;
}

void MapCreator::back_to_main_menu()
{
    bool map_creator_is_closed = this -> close();
    if(!(map_creator_is_closed))
    {
        QMessageBox::critical(this, NULL, "Map creator could not be closed.");
        return;
    }

    MainWindow *menu = new MainWindow;
    menu -> show();
}

void MapCreator::request_map_saving()
{
    bool required_components_are_placed = assert_that_required_components_are_placed();

    if(!(required_components_are_placed))
    {
        QMessageBox::warning(this, NULL, "Required map components, entrance and exit, are not placed.");
        return;
    }

    fill_map_lemmings_parameters(this -> map);

    if(lemmings_configuration_is_set == false)
    {
        QMessageBox::warning(this, NULL, "Required values were not provided.");
        return;
    }

    MapConversionManager::Get().process_map_saving_request(this);
}

void MapCreator::start_game()
{
    bool required_components_are_placed = assert_that_required_components_are_placed();

    if(!(required_components_are_placed))
    {
        QMessageBox::warning(this, NULL, "Required map components, entrance and exit, are not placed.");
        return;
    }

    Map *map_without_gui = prepare_map_without_gui();
    if(map_without_gui == nullptr)
    {
        QMessageBox::warning(this, NULL, "Required values were not provided.");
        return;
    }
    Game::Get().initialize_game(map_without_gui);

    this -> close();
}

Map *MapCreator::prepare_map_without_gui()
{
    //TODO: Check this function, propably just deleting items on GUI panel will be faster
    Map *map_without_gui = new Map;
    fill_map_lemmings_parameters(map_without_gui);

    if(lemmings_configuration_is_set == false)
    {
        map_without_gui = nullptr;
        return map_without_gui;
    }

    QList<QGraphicsItem*> components_list = map -> items(0, 0, map -> get_width(), map -> get_gui_panel_boundary_y_pos(),
                                                         Qt::ContainsItemShape, Qt::DescendingOrder, QTransform());
    foreach(const QGraphicsItem* component, components_list)
    {
        if(component -> type() == Block::Type)
        {
            map_without_gui -> add_block(component -> x(), component -> y());

        }
        else if(component -> type() == Exit::Type)
        {
            map_without_gui -> add_exit(component -> x(), component -> y());
        }
        else
        {
            map_without_gui -> add_entrance(component -> x(), component -> y());
        }
    }

    return map_without_gui;
}

void MapCreator::set_map_lemmings_parameters(QList<QLineEdit*> parameters_to_fill_line_edits, Map *map)
{
    map -> available_lemmings_class_changes_list.clear();
    for(int i = 0; i < parameters_to_fill_line_edits.size() - 2; i ++)
        map -> available_lemmings_class_changes_list.append(parameters_to_fill_line_edits[i] -> text().toInt());


    map -> lemmings_to_spawn = parameters_to_fill_line_edits[parameters_to_fill_line_edits.size() - 2] -> text().toInt();
    map -> lemmings_to_save = parameters_to_fill_line_edits.last() -> text().toInt();
    lemmings_configuration_is_set = true;
}

void MapCreator::prepare_dialog_buttons(QFormLayout *form, QDialog *dialog)
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal, dialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));

    form -> addRow(buttonBox);
}

void MapCreator::add_dialog_line_edit_int_validated(QDialog *dialog, QList<QLineEdit*> *line_edits_list, QString label,
                                                    QFormLayout *form, int validator_limit, int parameter_index)
{
    QLineEdit *line_edit = new QLineEdit(dialog);
    line_edit -> setValidator( new QIntValidator(0, validator_limit, this) );
    form -> addRow(label, line_edit);
    line_edits_list->append(line_edit);

    if(map -> available_lemmings_class_changes_list.isEmpty() == false)
    {
        if(parameter_index < 7)
        {
            const QString temp = QString::number(map -> available_lemmings_class_changes_list[parameter_index]);
            line_edit -> setText(temp);
        }
        else if(parameter_index == 7)
        {
            const QString temp = QString::number(map -> lemmings_to_spawn);
            line_edit -> setText(temp);
        }
        else if(parameter_index == 8)
        {
            const QString temp = QString::number(map -> lemmings_to_save);
            line_edit -> setText(temp);
        }

        lemmings_configuration_is_set = true;
    }
}

void MapCreator::fill_map_lemmings_parameters(Map *map)
{
    QList<QLineEdit*> parameters_to_fill_line_edits;
    QDialog dialog(this);
    QFormLayout form(&dialog);

    int lemmings_classes_implemented = 7;
    int lemmings_survival_parameters = 2;
    int parameters_to_fill = lemmings_classes_implemented + lemmings_survival_parameters;
    QList<QString> parameters_labels = { "Blocker", "Climber", "Paratrooper", "Basher", "Digger", "Bridge builder", "Ramp builder",
                                         "Lemmings to spawn", "Lemmings to save"};

    int validator_limit = 100;
    form.addRow(new QLabel("Available lemmings class changes:"));
    for(int i = 0; i < parameters_to_fill; ++i)
    {
        if(i == lemmings_classes_implemented)
        {
            form.addRow(new QLabel("Lemmings survival rate:"));
            validator_limit = 9;
        }
        add_dialog_line_edit_int_validated(&dialog, &parameters_to_fill_line_edits, parameters_labels[i], &form, validator_limit, i);
    }

    prepare_dialog_buttons(&form, &dialog);

    if (dialog.exec() == QDialog::Accepted)
    {
        foreach(QLineEdit* line_edit, parameters_to_fill_line_edits)
            if(line_edit -> text().isEmpty())
            {
                lemmings_configuration_is_set = false;
                return;
            }

        set_map_lemmings_parameters(parameters_to_fill_line_edits, map);
    }
    else
    {
        lemmings_configuration_is_set = false;
    }
}
