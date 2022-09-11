#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

//Sigleton game class for global access
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

    int scene_width = 1760;
    int scene_height = 960;
    QGraphicsScene *scene;

};


#endif // GAME_H

