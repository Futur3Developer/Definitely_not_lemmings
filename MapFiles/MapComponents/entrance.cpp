#include "entrance.h"
#include "Lemmings/joblesslemming.h"
#include "game.h"

#include <QCoreApplication>
#include <QTime>

Entrance::Entrance()
{
    setRect(0,0,90,120);

    QLinearGradient lgrad(this->boundingRect().topLeft(), this->boundingRect().bottomLeft());

    lgrad.setColorAt(0.0, Qt::darkBlue);
    lgrad.setColorAt(1.0, Qt::blue);

    this->setBrush(lgrad);
}

Entrance::~Entrance()
{

}


void Entrance::spawn_lemming(bool delay_should_be_applied)
{
    if(delay_should_be_applied)
        delay_spawn(1500);

    Map* map = Game::Get().get_map();
    QList<Lemming*> *lemmings_alive = Game::Get().get_lemmings_alive();
    QPointF entrance_pos = this -> pos();
    int entrance_height = this -> boundingRect().height();

    Lemming *lemming = new JoblessLemming();
    lemming -> setPos(entrance_pos.x(), entrance_pos.y() + entrance_height/2 - 5);
    map -> addItem(lemming);
    lemmings_alive -> append(lemming);

}

void Entrance::delay_spawn(int delay_time_in_milliseconds)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(delay_time_in_milliseconds);
    loop.exec();
}
