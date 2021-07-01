#include "Enemy/Scout.h"

Scout::Scout(int idx, int row, int col, int level) : Enemy(idx, row, col, level)
{
    health = level_health[level];
    max_health = level_health[level];
    speed = level_speed[level];
    gold = level_gold[level];
    type = EnemyType::SCOUT;
}

QPixmap Scout::get_enemy_pixmap() const {
    switch(level) {
    default: { return QPixmap("");}
    case 0 ... 4: { return QPixmap(":/images/enemy_images/Scout/Scout_1.png");}
    case 5 ... 8: { return QPixmap(":/images/enemy_images/Scout/Scout_2.png");}
    case 9: { return QPixmap(":/images/enemy_images/Scout/Scout_3.png");}
    }
}

