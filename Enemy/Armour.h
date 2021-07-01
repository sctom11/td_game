#ifndef ARMOUR_H
#define ARMOUR_H

#include "Enemy/enemy.h"

class Armour : public Enemy
{
private:
    const int level_health[10] = {100, 120, 150, 190, 250, 320, 370, 490, 600, 900};
    const int level_speed[10] = {25, 24, 23, 22, 21, 20, 19, 18, 17, 16};
    const int level_gold[10] = {2, 2, 2, 3, 3, 3, 3, 4, 4, 4};

    friend class GameMap;
    friend class Tower;
    friend class MainWindow;

public:
    Armour(int idx, int row, int col, int level);
    virtual QPixmap get_enemy_pixmap() const override;

};

#endif // ARMOUR_H
