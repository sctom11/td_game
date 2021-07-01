#include "Enemy/enemy.h"


Enemy::Enemy(int idx, int row, int col, int level) : idx(idx), level(level), position_row(row), position_col(col){
    effect = new QGraphicsColorizeEffect(this);
    effect -> setColor(QColor(100,0,0));
    effect -> setEnabled(true);
}

Enemy::~Enemy() {
    delete view;
}

int Enemy::get_health() const {return health;}
int Enemy::get_max_health() const {return max_health;}
int Enemy::get_speed() const {return speed;}
int Enemy::get_gold() const {return gold;}
int Enemy::get_position_col() const {return position_col;}
int Enemy::get_position_row() const {return position_row;}
Enemy::EnemyType Enemy::get_enemy_type() const {return type;}
bool Enemy::is_alive() const {return alive;}

void Enemy::receive_damage(unsigned int damage) {
    health -= damage;
    effect -> setEnabled(true);
    image -> setGraphicsEffect(effect);
    QTimer* tmp = new QTimer(this);
    connect(tmp, SIGNAL(timeout()), this,SLOT(reset_effect()));
    tmp -> setSingleShot(true);
    tmp -> start(200);
    if (health < 0) {
        health = 0;
        delete view;

        alive = false;
    }
}

void Enemy::reset_effect()
{
    effect -> setEnabled(false);
}


