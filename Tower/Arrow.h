#ifndef ARROW_H
#define ARROW_H

#include "Tower/Tower.h"

class Arrow : public Tower {
private:
    const int _tower_attack_damage[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    const int _tower_attack_range[10] = {4, 4, 5, 5, 5, 6, 6, 7, 7, 8};
    const int _tower_attack_interval[10] = {400, 380, 360, 340, 330, 320, 310, 300, 300, 300};
public:
    Arrow(int row, int col);
    ~Arrow() override;

    // Increase the damage depending on tower level
    virtual bool tower_level_up() override;
    virtual QPixmap get_tower_pixmap() const override;
    virtual QString tower_name_string() const override;

private slots:
    virtual void attack() override;
};

#endif //ARROW_H
