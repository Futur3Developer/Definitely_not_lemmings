#include "entrance.h"

Entrance::Entrance()
{
    setRect(0,0,90,120);

    QLinearGradient lgrad(this->boundingRect().topLeft(), this->boundingRect().bottomLeft());

    lgrad.setColorAt(0.0, Qt::darkBlue);
    lgrad.setColorAt(1.0, Qt::blue);

    this->setBrush(lgrad);
    //this->setAcceptHoverEvents(true);
}
