#include "Player.h"
#include "Tower/Tower.h"
#include "Tower/Arrow.h"
#include "Tower/Cannon.h"
#include "Tower/Wizard.h"

Player::Player(){}

Player::~Player(){}

//Accessor
int Player::get_money() const{return money;}
int Player::get_num_tower() const{return num_tower;}
int Player::get_max_tower() const{return max_tower;}
Tower** Player::get_towers() const {return towers;}
int Player::get_health() const {return health;}
int Player::get_max_health() const {return max_health;}

void Player::set_money(int money) {
    this->money = money;
}

void Player::destroy_tower(Tower* tower) {
    // Loop through the lists of tower that Player owned
    for(int i = 0; i < num_tower; ++i) {
        // Found the tower
        if(towers[i] == tower) {
            // Delete tower[i]
            delete towers[i];
            /* TODO
             * remove the tower from the treeWidget and in the GUI
             */

            // Move forward the list after the deleted tower
            for(int j = i; j < num_tower - 1; ++j) {
                towers[j] = towers[j+1];
            }
            towers[num_tower - 1] = nullptr;
            --num_tower;
        }
    }
}

// Place the tower by row, col and tower_type
void Player::place_tower(int row, int col, Tower::TowerType tower_type)
{
    switch(tower_type) {
    case Tower::TowerType::ARROW: {
        Tower* new_tower = new Arrow(row, col);
        towers[num_tower] = new_tower;
        num_tower++;

        break;
    }
    case Tower::TowerType::CANNON: {
        Tower* new_tower = new Cannon(row, col);
        towers[num_tower] = new_tower;
        num_tower++;

        break;
    }
    case Tower::TowerType::WIZARD: {
        Tower* new_tower = new Wizard(row, col);
        towers[num_tower] = new_tower;
        num_tower++;

    }
    }
}


void Player::reset_focus()
{
    for (int idx = 0; idx < num_tower; ++idx)
    {
        towers[idx] -> current_focus_enemy.clear();
    }
}
