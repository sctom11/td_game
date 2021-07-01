#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTreeWidgetItem>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>


#include "GameMap.h"
#include "Player.h"
#include "Tower/Tower.h"
#include "Enemy/enemy.h"
#include "Enemy/Scout.h"
#include "Enemy/Armour.h"
#include "Enemy/Warmachine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_released();
    void on_upgradeButton_released();
    void on_destroyButton_released();
    void on_confirmButton_released();
    void on_cancelButton_released();
    void on_buyarrowButton_released();
    void on_buycannonButton_released();
    void on_buywizardButton_released();

    //void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

    void map_clicked(int row, int col);
    //For game_clock
    void count_down();

    //For spawning enemies
    void spawn_enemy();

    //For rendering enemies movement
    void render_gui(int idx);

private:
    enum stats_display {
        TOWER_STATS,
        TYPE,
        LEVEL,
        DAMAGE,
        ATTACK_RANGE,
        ATTACK_SPEED,
        COST
    };

    //For rendering enemy movement
    void start_rendering_enemies_gui();
    //For rendering enemy movement

    bool is_game_over() const;
    void get_tower_by_pos(int row, int col, int &tower_index_out);

    // A boolean value to determine whether the round is over
    bool round_over = true;

    // Called when the game has ended
    void game_end(bool win=true);

    // Called when a round has ended
    void end_round();

    // Update the pixmap for wave
    void update_wave_pixmap();

    // To initialize the stats view
    void stats_view_initialization();

    // To update the stats view
    void update_stats_view();

    // To update the health display
    void update_health_display();

    //Check is the enemy in the tower range
    bool check_enemy_in_range(Enemy* enemy, Tower* tower) const;

    // Set the enability of the buy tower buttons
    void set_buytower_button_enabled(bool status);

    //For generating random enemies
    Enemy* generate_random_enemies(int idx);

    GameMap game_map;
    Player* player{nullptr};
    //QTreeWidgetItem* tower_list{nullptr};
    QList<Enemy*> enemies_on_map;
    QList<Enemy*> total_enemies;
    QList<QTimer*> enemies_clock{nullptr};
    QTimer* spawn_clock{nullptr};

    //Game time
    QTimer* game_clock{nullptr};
    int remaining_time{150}; //In second
    int current_wave{0};
    int max_wave{9};

    QGraphicsScene scene;
    QGraphicsScene wave_scene;
    QGraphicsScene gold_scene;
    QGraphicsScene shop_scene;
    QGraphicsScene stats_scene;

    // Pixmap for Graphics view
    QGraphicsPixmapItem* wavePixmap;
    QGraphicsPixmapItem* goldPixmap;
    QGraphicsPixmapItem* shopPixmap;

    // Pixmap for Tower image display inside stats view
    QGraphicsPixmapItem* stats_displayed_tower;

    // list of text item for stats view
    QList<QGraphicsTextItem*> stats_text;

    // Health text to be displayed
    QGraphicsTextItem* health_display;

    int selected_tower{-1};
    int confirming_tower{-1};
    bool is_placing_tower{false};
    Tower::TowerType placing_tower_type{Tower::TowerType::ARROW};


    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
