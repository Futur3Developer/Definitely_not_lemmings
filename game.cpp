#include "game.h"

#include <QGraphicsItem>

Game::Game():QGraphicsView(){
    // Creating scene
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,scene_width,scene_height);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(scene_width,scene_height);

    setScene(scene);
    this->show();
}
