#include "score.h"
#include "game.h"

#include <QFont>
#include <QDebug>

Score::Score(QGraphicsItem *parent): QGraphicsTextItem(parent)
{
    saved_lemmings = 0;
    dead_lemmings = 0;
    lemmings_to_save = Game::Get().get_lemmings_to_save();
    spawned_lemmings = Game::Get().get_lemmings_to_spawn();

    acceptable_lemmings_casualty = spawned_lemmings - lemmings_to_save;

    setPlainText(QString("Lemmings saved: ") + QString::number(saved_lemmings));
    setDefaultTextColor(Qt::darkGreen);
    setFont(QFont("times",16, QFont::Bold));

    if(acceptable_lemmings_casualty < 0)
        show_text_after_losing_the_game();
}

void Score::increase_saved_lemmings_counter()
{
    saved_lemmings++;
    setPlainText(QString("Lemmings saved:") + QString::number(saved_lemmings));

    if(saved_lemmings == lemmings_to_save)
        show_text_after_winning_the_game();
}

void Score::increase_dead_lemmings_counter()
{
    qDebug()<<"Dead lemmings increased";
    dead_lemmings++;
    qDebug()<<"Dead lemmings" << dead_lemmings;
    if(dead_lemmings >= (acceptable_lemmings_casualty + 1))
        show_text_after_losing_the_game();
}

void Score::show_text_after_winning_the_game()
{
    QGraphicsTextItem *win = new QGraphicsTextItem();
    win -> setPlainText(QString("You won!"));
    win -> setDefaultTextColor(Qt::darkGreen);
    win -> setFont(QFont("times",50));

    int x = Game::Get().get_map() -> get_width()/2 - win -> boundingRect().width()/2;
    int y = Game::Get().get_map() -> get_height()/2 - Game::Get().get_map() -> get_height() -
            Game::Get().get_map() -> get_gui_panel_boundary_y_pos();

    win -> setPos(x, y);
    Game::Get().get_map() -> addItem(win);
}

void Score::show_text_after_losing_the_game()
{
    QGraphicsTextItem *lose = new QGraphicsTextItem();
    lose -> setPlainText(QString("You lost!"));
    lose -> setDefaultTextColor(Qt::darkBlue);
    lose -> setFont(QFont("times",50));

    int x = Game::Get().get_map() -> get_width()/2 - lose -> boundingRect().width()/2;
    int y = Game::Get().get_map() -> get_height()/2;

    lose -> setPos(x, y);
    Game::Get().get_map() -> addItem(lose);
}
