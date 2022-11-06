#ifndef GAME_H
#define GAME_H

#include "mapcreator.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

//Singleton game class for global access
class Game: public QGraphicsView{
    Q_OBJECT
public:
    Game(const Game&) = delete;

    static Game& Get()
    {
        static Game game_Instance;
        return game_Instance;
    }

private:

    Game();

    //QList<Lemming*> lemmings_alive;
    int scene_width = 1760;
    int scene_height = 960;
    QGraphicsScene *scene;
    Map *game_map;

};


#endif // GAME_H

