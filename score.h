#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>
#include <QTime>

class Score: public QGraphicsTextItem
{
public:
    Score(QGraphicsItem *parent=0);
    void increase_saved_lemmings_counter();
    void increase_dead_lemmings_counter();

private:
    void show_text_after_winning_the_game();
    void show_text_after_losing_the_game();

    int saved_lemmings;
    int lemmings_to_save;
    int spawned_lemmings;
    int dead_lemmings;
    int acceptable_lemmings_casualty;
};

#endif // SCORE_H

