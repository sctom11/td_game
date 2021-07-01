#ifndef WARMACHINE_H
#define WARMACHINE_H

#include "Enemy/enemy.h"

class WarMachine : public Enemy
{
private:
    // Enemy stats based on level
    const int level_health[10] = {250, 300, 350, 410, 460, 500, 550, 650, 800, 1200};
    const int level_speed[10] = {35, 35, 35, 34, 34, 34, 34, 33, 33, 33};
    const int level_gold[10] = {3, 3, 3, 3, 4, 4, 4, 4, 5, 5};

    friend class GameMap;
    friend class Tower;
    friend class MainWindow;

public:
    WarMachine(int idx, int row, int col, int level);
    virtual QPixmap get_enemy_pixmap() const override;

};

#endif // WARMACHINE_H
