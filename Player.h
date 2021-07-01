#ifndef PLAYER_H_
#define PLAYER_H_

#include "Tower/Tower.h"
#include <string>
#include <QList>
#include <QTreeWidgetItem>
#include <QString>

class Player
{
private:
    int money{50};
    int num_tower{0};
    int health{30};
    int max_health{30};
    int max_tower{80};

    Tower** towers = new Tower* [max_tower];
    QTreeWidgetItem *list_item{nullptr};

public:
    Player();
    ~Player();

    //Accessor
    int get_money() const;
    Tower** get_towers() const;
    int get_num_tower() const;
    int get_max_tower() const;

    void set_money(int money);

    int get_health() const;
    int get_max_health() const;

    // Tower
    void destroy_tower(Tower* tower);
    void place_tower(int row, int col, Tower::TowerType tower_type);
    void reset_focus();

    friend class MainWindow;

};

#endif /* PLAYER_H_ */

