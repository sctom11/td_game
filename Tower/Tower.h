#ifndef TOWER_H
#define TOWER_H

#include <QTimer>
#include <QGraphicsLineItem>
#include <QList>
#include "Enemy/enemy.h"
#include <QDebug>

#include <cmath>

class Tower : public QObject
{
    Q_OBJECT
public:
    enum class TowerType {ARROW, CANNON, WIZARD};

    Tower(TowerType tower_type, int tower_cost, int tower_upgrade_cost, int tower_level, int row, int col);
    virtual ~Tower();

    //Enemy* get_current_focus() const;
    double get_last_attack() const;

    TowerType get_tower_type() const;
    int get_tower_cost() const;
    int get_tower_upgrade_cost() const;
    int get_tower_level() const;
    int get_tower_damage() const;
    int get_tower_range() const;
    int get_tower_attack_speed() const;
    int get_tower_row() const;
    int get_tower_col() const;

    virtual QPixmap get_tower_pixmap() const = 0;
    virtual bool tower_level_up() = 0;
    virtual QString tower_name_string() const = 0;

    friend class MainWindow;
    friend class Player;

private slots:
    virtual void attack() = 0;

protected:

    TowerType tower_type;

    QList<Enemy*> current_focus_enemy;

    int tower_cost;
    int tower_upgrade_cost;
    int tower_level;
    int tower_damage;
    int tower_range;
    int tower_attack_speed;
    int position_row, position_col;
    QTimer* attack_timer{nullptr};
    QList<QGraphicsLineItem*> line{nullptr};
};

#endif //TOWER_H

