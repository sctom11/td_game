#include "Tower/Wizard.h"


//Tower::Tower(TowerType tower_type, int tower_cost, int upgrade_cost, int tower_level, int row, int col)
Wizard::Wizard(int row, int col) : Tower(Tower::TowerType::WIZARD, 10, 7, 0, row, col) {
    tower_damage = _tower_attack_damage[0];
    tower_range = _tower_attack_range[0];
    tower_attack_speed = _tower_attack_interval[0];

    attack_timer = new QTimer(this);
    connect(attack_timer, SIGNAL(timeout()), this, SLOT(attack()));
    attack_timer -> start(tower_attack_speed); //speed 1000


}

Wizard::~Wizard() {}

bool Wizard::tower_level_up() {
    if(tower_level >= 9) {return false;}

    tower_level += 1;
    tower_damage = _tower_attack_damage[tower_level];
    tower_range = _tower_attack_range[tower_level];
    tower_attack_speed = _tower_attack_interval[tower_level];
    return true;
}

void Wizard::attack()
{
    if (current_focus_enemy.isEmpty())
        return;



    for (int idx = 0; idx < current_focus_enemy.size(); ++idx)
    {
        if(idx > _tower_attack_target[tower_level]) break;
        current_focus_enemy[idx] -> receive_damage((_tower_attack_damage[tower_level] * pow(0.7, idx)));
    }
}

QString Wizard::tower_name_string() const {
    return QString("Wizard");
}

QPixmap Wizard::get_tower_pixmap() const {
    switch(tower_level) {
    default: {return QPixmap("");}
    case 0 ... 4: { return QPixmap(":/images/tower_images/Wizard/Wizard_1.png"); }
    case 5 ... 8: { return QPixmap(":/images/tower_images/Wizard/Wizard_2.png"); }
    case 9: { return QPixmap(":/images/tower_images/Wizard/Wizard_3.png"); }
    }
}
