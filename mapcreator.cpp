#include "mapcreator.h"
#include "mapconversionmanager.h"
#include "mainwindow.h"

MapCreator::MapCreator()
{
    map = new Map();

    set_view_configuration();
    initialize_map_components();
    add_map_creation_guidelines();
    add_buttons();
}

MapCreator::MapCreator(Map *map)
{
    this -> map = map;

    set_view_configuration();
    initialize_map_components();
    add_map_creation_guidelines();
    add_buttons();
}

Map *MapCreator::get_map()
{
    return map;
}

void MapCreator::set_map(Map *map)
{
    this -> map = map;
}

void MapCreator::set_view_configuration()
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(map -> get_width(), map -> get_height());
    setScene(map);
}

void MapCreator::initialize_map_components()
{
    block_original_position.setX(map -> get_width()/2 - 40);
    block_original_position.setY(map -> get_height() - 120);

    entrance_original_position.setX(block_original_position.x() - 300);
    entrance_original_position.setY(block_original_position.y() - 20);

    exit_original_position.setX(block_original_position.x() + 300);
    exit_original_position.setY(block_original_position.y() - 20);

    map -> add_block(block_original_position.x(), block_original_position.y());
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
    //connect...

    QPushButton *save_map_button = new QPushButton();
    add_button(save_map_button, map -> get_width() - 270, "Save map");
    connect(save_map_button, &QPushButton::clicked, this, &MapCreator::request_map_saving);
}

void MapCreator::mousePressEvent(QMouseEvent *event)
{
    QGraphicsItem *clicked_item = itemAt(event ->pos());

    if((event ->buttons() & Qt::LeftButton) && !(clicked_item == NULL) && component_is_being_placed == false)
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
        this -> map ->add_block(map -> get_width()/2 - 40, map -> get_height() - 120);

}

bool MapCreator::assert_that_position_is_valid(QPointF new_position)
{
    if(component_to_place -> type() == Block::Type)
    {
        if(new_position.y() < map -> get_height() - gui_boundary_height - 80 && new_position.x() < map -> get_width() - 80)
            return true;
        return false;
    }
    else if (new_position.y() < map -> get_height() - gui_boundary_height - 120 && new_position.x() < map -> get_width() - 90)
        return true;
    return false;
}

bool MapCreator::check_if_there_is_new_block_to_place()
{
    QGraphicsItem *item_at_block_position = map -> itemAt(block_original_position, QTransform());

    if(item_at_block_position == NULL)
    {
        return false;
    }

    return true;
}

void MapCreator::remove_component_from_map()
{
    component_is_being_placed = false;

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
    if((map -> itemAt(entrance_original_position, QTransform()) != NULL) || (map -> itemAt(exit_original_position, QTransform()) != NULL))
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

    MapConversionManager::Get().process_map_saving_request(this);
}

void MapCreator::start_game()
{
    this -> close();
}
