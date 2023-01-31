#include "score.h"
#include "game.h"

#include <QFont>

void Score::add_saved_lemmings_text()
{
    setPlainText(QString("Lemmings saved: ") + QString::number(saved_lemmings));
    setDefaultTextColor(Qt::darkBlue);
    setFont(QFont("times",16, QFont::Bold));
}

void Score::add_remaining_lemmings_to_save_text()
{
    remaining_lemmings_to_save_text -> setPlainText(QString("Remaining lemmings to save: ") + QString::number(lemmings_to_save));
    remaining_lemmings_to_save_text -> setDefaultTextColor(Qt::darkBlue);
    remaining_lemmings_to_save_text -> setFont(QFont("times", 16, QFont::Bold));

    int x = Game::Get().get_map() -> get_width()/2 - remaining_lemmings_to_save_text -> boundingRect().width()/2;

    remaining_lemmings_to_save_text -> setPos(x, 10);
    Game::Get().get_map() -> addItem(remaining_lemmings_to_save_text);
}

Score::Score(QGraphicsItem *parent): QGraphicsTextItem(parent)
{
    remaining_lemmings_to_save_text = new QGraphicsTextItem();

    saved_lemmings = 0;
    dead_lemmings = 0;
    lemmings_to_save = Game::Get().get_lemmings_to_save();
    spawned_lemmings = Game::Get().get_lemmings_to_spawn();
    acceptable_lemmings_casualty = spawned_lemmings - lemmings_to_save;
    remaining_lemmings_to_save = lemmings_to_save - saved_lemmings;

    add_saved_lemmings_text();
    add_remaining_lemmings_to_save_text();

    if(acceptable_lemmings_casualty < 0)
        show_text_after_losing_the_game();

    if(saved_lemmings == lemmings_to_save)
        show_text_after_winning_the_game();
}


Score::~Score()
{
    delete remaining_lemmings_to_save_text;
}

void Score::decrease_remaining_lemmings_to_save_counter()
{
    remaining_lemmings_to_save_text -> setPlainText(QString("Remaining lemmings to save: ") + QString::number(remaining_lemmings_to_save));
}

void Score::increase_saved_lemmings_counter()
{
    saved_lemmings++;
    setPlainText(QString("Lemmings saved: ") + QString::number(saved_lemmings));

    if(saved_lemmings == lemmings_to_save)
        show_text_after_winning_the_game();


    remaining_lemmings_to_save--;
    if(remaining_lemmings_to_save >= 0)
        decrease_remaining_lemmings_to_save_counter();
}

void Score::increase_dead_lemmings_counter()
{
    dead_lemmings++;
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
    int y = Game::Get().get_map() -> get_height()/2;

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
