#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>
#include <QTime>

class Score: public QGraphicsTextItem
{
public:
    Score(QGraphicsItem *parent=0);
    ~Score();
    void increase_saved_lemmings_counter();
    void increase_dead_lemmings_counter();

private:
    void show_text_after_winning_the_game();
    void show_text_after_losing_the_game();
    void add_lemmings_to_save_text();
    void add_saved_lemmings_text();
    void add_remaining_lemmings_to_save_text();
    void decrease_remaining_lemmings_to_save_counter();

    int saved_lemmings;
    int lemmings_to_save;
    int spawned_lemmings;
    int dead_lemmings;
    int acceptable_lemmings_casualty;
    int remaining_lemmings_to_save;
    QGraphicsTextItem *remaining_lemmings_to_save_text;


};

#endif // SCORE_H

