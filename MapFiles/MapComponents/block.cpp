#include "block.h"

Block::Block()
{
    setRect(0,0,80, 80);

    QBrush brush1;
    brush1.setStyle(Qt::SolidPattern);
    brush1.setColor(Qt::lightGray);
    setBrush(brush1);
}

