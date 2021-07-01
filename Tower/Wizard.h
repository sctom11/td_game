#ifndef WIZARD_H
#define WIZARD_H

#include "Tower/Tower.h"

class Wizard : public Tower {
private:
    const int _tower_attack_damage[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    const int _tower_attack_range[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
    const int _tower_attack_interval[10] = {1000, 980, 960, 940, 920, 900, 880, 860, 840, 820};
    const int _tower_attack_target[10] = {2, 2, 2, 3, 3, 3, 3, 4, 4, 5};
public:
    Wizard(int row, int col);
    ~Wizard() override;

    // Increase the damage depending on tower level
    virtual bool tower_level_up() override;
    virtual QPixmap get_tower_pixmap() const override;
    virtual QString tower_name_string() const override;

private slots:
    virtual void attack() override;
};

#endif //WIZARD_H
