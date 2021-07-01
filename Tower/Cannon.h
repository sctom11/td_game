#ifndef CANNON_H
#define CANNON_H

#include "Tower/Tower.h"

class Cannon : public Tower {
private:
    const int _tower_attack_damage[10] = {12, 14, 16, 19, 21, 24, 27, 30, 33, 36};
    const int _tower_attack_range[10] = {7, 7, 8, 8, 8, 8, 9, 9, 9, 9};
    const int _tower_attack_interval[10] = {1000, 950, 950, 950, 900, 900, 850, 800, 800, 750};
public:
    Cannon(int row, int col);
    ~Cannon() override;

    // Increase the damage depending on tower level
    virtual bool tower_level_up() override;
    virtual QPixmap get_tower_pixmap() const override;
    virtual QString tower_name_string() const override;

private slots:
    virtual void attack() override;

};

#endif //CANNON_H
