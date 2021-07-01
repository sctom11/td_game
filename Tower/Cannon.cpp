#include "Tower/Cannon.h"

//Tower::Tower(TowerType tower_type, int tower_cost, int upgrade_cost, int tower_level, int row, int col)
Cannon::Cannon(int row, int col) : Tower(Tower::TowerType::CANNON, 10, 5, 0, row, col) {
    tower_damage = _tower_attack_damage[0];
    tower_range = _tower_attack_range[0];
    tower_attack_speed = _tower_attack_interval[0];

    attack_timer = new QTimer(this);
    connect(attack_timer, SIGNAL(timeout()), this, SLOT(attack()));
    attack_timer -> start(tower_attack_speed); //speed 1000


}

Cannon::~Cannon() {}

bool Cannon::tower_level_up() {
    if(tower_level >= 9) {return false;}


    tower_level += 1;
    tower_damage = _tower_attack_damage[tower_level];
    tower_range = _tower_attack_range[tower_level];
    tower_attack_speed = _tower_attack_interval[tower_level];
    return true;
}

void Cannon::attack()
{
    if (current_focus_enemy.isEmpty())
        return;



    int damage = _tower_attack_damage[tower_level];
    if (current_focus_enemy[0] -> get_enemy_type() == Enemy::EnemyType::WARMACHINE)
        damage *= 1.5;
    damage = floor(damage);
    current_focus_enemy[0] -> receive_damage(damage);
}

QString Cannon::tower_name_string() const {
    return QString("Cannon");
}

QPixmap Cannon::get_tower_pixmap() const {
    switch(tower_level) {
    default: {return QPixmap("");}
    case 0 ... 4: { return QPixmap(":/images/tower_images/Cannon/Cannon_1.png"); }
    case 5 ... 8: { return QPixmap(":/images/tower_images/Cannon/Cannon_2.png"); }
    case 9: { return QPixmap(":/images/tower_images/Cannon/Cannon_3.png"); }
    }
}
