#ifndef SCOUT_H
#define SCOUT_H

#include "Enemy/enemy.h"

class Scout : public Enemy
{
private:
    // Enemy stats based on level
    const int level_health[10] = {50, 60, 80, 110, 130, 150, 150, 170, 200, 250};
    const int level_speed[10] = {12, 12, 11, 11, 10, 10, 9, 9, 8, 7};
    const int level_gold[10] = {1, 1, 2, 2, 2, 3, 3, 3, 4, 4};

    friend class GameMap;
    friend class Tower;
    friend class MainWindow;

public:
    Scout(int idx, int row, int col, int level);
    virtual QPixmap get_enemy_pixmap() const override;

};

#endif // SCOUT_H
