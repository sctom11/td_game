#include "Enemy/Warmachine.h"

WarMachine::WarMachine(int idx, int row, int col, int level) : Enemy(idx, row, col, level)
{
    health = level_health[level];
    max_health = level_health[level];
    speed = level_speed[level];
    gold = level_gold[level];
    type = EnemyType::WARMACHINE;
}

QPixmap WarMachine::get_enemy_pixmap() const {
    switch(level) {
    default: { return QPixmap("");}
    case 0 ... 4: { return QPixmap(":/images/enemy_images/WarMachine/WarMachine_1.png");}
    case 5 ... 8: { return QPixmap(":/images/enemy_images/WarMachine/WarMachine_2.png");}
    case 9: { return QPixmap(":/images/enemy_images/WarMachine/WarMachine_3.png");}
    }
}
