#ifndef GAMEMAP_H_
#define GAMEMAP_H_

#include <string>
using std::string;

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QList>
#include <cmath>

#include "Tower/Tower.h"
#include "Player.h"


class GameMap {
public:
    enum class TerrainState { SPACE, UP, DOWN, LEFT, RIGHT, BLOCK, DECO};
    enum class EnemiesTowersState {SPACE, TOWER, ENEMY};

    GameMap();
    ~GameMap();

    //void load_terrain_map(const string& filename);
    void update_terrain_map(int row, int col, TerrainState state);
    void update_tower_and_enemies_map(int row, int col, EnemiesTowersState state);
    void render_map_gui(QGraphicsScene &scene);
    void render_tower_gui(QGraphicsScene &scene, Player* player);
    void indicate_area(int row, int col, int rad);
    void reset_indicators();

    int get_num_rows() const;
    int get_num_cols() const;

    int get_image_size() const;

    bool build_on_terrain(int row, int col);

    //For rendering GUI in real-time

    void render_enemy_gui(QGraphicsScene &scene, Enemy *enemies_on_map);
    void render_enemy_movement(Enemy *enemies_on_map, int idx);

private:
    const int num_rows{20};
    const int num_cols{30};
    int spawn_row{17};
    int spawn_col{0};
    int goal_row{17};
    int goal_col{29};
    TerrainState* terrain_map{nullptr};
    EnemiesTowersState* tower_and_enemies_map{nullptr};
    const int image_size{45}; //The dimension of the image (Assume square)


    // Return a health bar pixmap depending on enemy's health
    QPixmap get_health_pixmap(Enemy* enemy);

    // Return the Pixmap for the tower
    QPixmap get_tower_pixmap(Tower* tower);


    QList<QGraphicsPixmapItem*> map_images;
    QList<QGraphicsRectItem*> map_indicators;
    QList<QGraphicsPixmapItem*> tower_images;

    //For rendering GUI in real-time
    QList<QGraphicsPixmapItem*> enemies_images;

    // (NEW) For rendering Health Bars in real-time
    QList<QGraphicsPixmapItem*> health_images;

    //For rendering GUI in real-time


    friend class MainWindow;
};

//Image
#endif /* GAMEMAP_H_ */
