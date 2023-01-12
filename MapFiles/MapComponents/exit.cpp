#include "exit.h"

Exit::Exit()
{
    setRect(0,0,90,120);

    QLinearGradient lgrad(this->boundingRect().topLeft(), this->boundingRect().bottomLeft());

    lgrad.setColorAt(0.0, Qt::darkGreen);
    lgrad.setColorAt(1.0, Qt::green);

    this->setBrush(lgrad);
    //this->setAcceptHoverEvents(true);
}
