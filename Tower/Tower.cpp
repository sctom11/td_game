#include "Tower/Tower.h"
#include <QTimer>

// Constructor and Destructor
Tower::Tower(TowerType tower_type, int tower_cost, int tower_upgrade_cost, int tower_level, int row, int col) :
            tower_type(tower_type), tower_cost(tower_cost), tower_upgrade_cost(tower_upgrade_cost),
            tower_level(tower_level), position_row(row), position_col(col)
{

}

Tower::~Tower() { }

Tower::TowerType Tower::get_tower_type() const { return tower_type;}
int Tower::get_tower_cost() const { return tower_cost;}
int Tower::get_tower_upgrade_cost() const { return tower_upgrade_cost;}
int Tower::get_tower_level() const { return tower_level;}
int Tower::get_tower_damage() const { return tower_damage;}
int Tower::get_tower_range() const { return tower_range;}

//Enemy* Tower::get_current_focus() const {
//    return current_focus_enemy;
//}

int Tower::get_tower_row() const { return position_row;}
int Tower::get_tower_col() const { return position_col;}
int Tower::get_tower_attack_speed() const{ return tower_attack_speed;}
