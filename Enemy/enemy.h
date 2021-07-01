#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QGraphicsEffect>

class Enemy : public QObject {
    Q_OBJECT;

private slots:
    void reset_effect();

public:
    enum class EnemyType{SCOUT, ARMOUR, WARMACHINE};
protected:
    int idx;
    int level;
    int health;
    int max_health;
    int speed;
    int gold;
    EnemyType type;

    bool on_map;
    bool alive{true};
    bool rewarded{false};
    int position_row; int position_col;
    int current_pixel{0};
    QGraphicsPixmapItem* image{nullptr};
    QGraphicsColorizeEffect* effect;

public:
    Enemy(int idx, int row, int col, int level);
    ~Enemy();


    QGraphicsPixmapItem *view{nullptr};
    QString get_id() const;
    int get_health() const;
    int get_max_health() const;
    int get_speed() const;
    int get_gold() const;
    int get_position_row() const;
    int get_position_col() const;
    EnemyType get_enemy_type() const;
    bool is_alive() const;
    void receive_damage(unsigned int damage);

    virtual QPixmap get_enemy_pixmap() const = 0;

    //virtual void refresh_list_item_status();

    friend class GameMap;
    friend class Tower;
    friend class MainWindow;
};

#endif // ENEMY_H
