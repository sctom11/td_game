#include "GameMap.h"

int GameMap::get_num_rows() const {return num_rows;}
int GameMap::get_num_cols() const {return num_cols;}
int GameMap::get_image_size() const {return image_size;}

GameMap::GameMap()
{
    // Hard coded game map layout
    int num_empty = 0;
    terrain_map = new TerrainState [num_rows*num_cols];
    tower_and_enemies_map = new EnemiesTowersState [num_rows*num_cols];
    for(int i = 0; i < num_rows; ++i) {
        for(int j = 0; j < num_cols; ++j) {
            if((i == 17 && (j <= 11 || j >= 18)) || (i == 2 && j >= 3 && j <= 26))
                terrain_map[i*num_cols+j] = TerrainState::RIGHT;
            else if((j == 3 && (i <= 12 && i >= 2)) || (j == 22 && (i <= 12 && i >= 8)) || (j == 12 && i >= 8 && i <= 17))
                terrain_map[i*num_cols+j] = TerrainState::UP;
            else if((i == 7 && ((j >= 9 && j <= 12) || (j >= 19 && j <= 22))) ||
                    (i == 12 && ((j >= 4 && j <= 8) || (j >= 23 && j <= 27))))
                terrain_map[i*num_cols+j] = TerrainState::LEFT;
            else if((j == 8 && i >= 7 && i <= 12) || (j == 18 && i >= 7 && i <= 16) || (j == 27 && i >= 2 && i <= 11))
                terrain_map[i*num_cols+j] = TerrainState::DOWN;
            else {
                terrain_map[i*num_cols+j] = TerrainState::SPACE;
                if(i != num_rows - 1 && j != num_cols - 1 && i != 0 && j != 0) {
                    ++num_empty;
                }
            }
            tower_and_enemies_map[i*num_cols+j] = EnemiesTowersState::SPACE;
        }
    }

    srand (time(NULL));
    int num_decoration = floor(num_empty * 0.05); // 5% of the empty spaces (where you can build tower) will be decorated
    for(int i = 0; i < num_decoration; ++i) {
        int deco_row;
        int deco_col;
        do {
            deco_row = rand() % num_rows;
            deco_col = rand() % num_cols;
        } while(terrain_map[deco_row * num_cols + deco_col] != TerrainState::SPACE ||
                deco_row == num_rows - 1 || deco_col == num_cols - 1 || deco_row == 0 || deco_col == 0);
        terrain_map[deco_row * num_cols + deco_col] = TerrainState::DECO;
    }
}

GameMap::~GameMap(){delete terrain_map; delete tower_and_enemies_map;}

//Render map GUI
void GameMap::render_map_gui(QGraphicsScene &scene)
{
    map_indicators.clear();
    map_images.clear();
    for (int row = 0; row < num_rows; ++row)
    {
        for (int col = 0; col < num_cols; ++col)
        {
            if (terrain_map[row * num_cols + col] == TerrainState::SPACE)
            {
                QGraphicsPixmapItem* pix_item = scene.addPixmap(QPixmap(":/images/map_images/space.bmp"));
                pix_item->setOffset(col * image_size, row * image_size);
                pix_item->setZValue(0);
                pix_item->setVisible(true);
                map_images.append(pix_item);
            }
            else if (terrain_map[row * num_cols + col] == TerrainState::BLOCK)
            {
                QGraphicsPixmapItem* pix_item = scene.addPixmap(QPixmap(":/images/map_images/blocked.bmp"));
                pix_item->setOffset(col * image_size, row * image_size);
                pix_item->setZValue(0);
                pix_item->setVisible(true);
                map_images.append(pix_item);
            }
            else if(terrain_map[row * num_cols + col] == TerrainState::DECO) {
                QPixmap deco_pixmap;
                int rand_pixmap = (rand() % 7) + 1;
                switch(rand_pixmap) {
                case 1: deco_pixmap.load(":/images/map_images/deco/deco_1.bmp"); break;
                case 2: deco_pixmap.load(":/images/map_images/deco/deco_2.bmp"); break;
                case 3: deco_pixmap.load(":/images/map_images/deco/deco_3.bmp"); break;
                case 4: deco_pixmap.load(":/images/map_images/deco/deco_4.bmp"); break;
                case 5: deco_pixmap.load(":/images/map_images/deco/deco_5.bmp"); break;
                case 6: deco_pixmap.load(":/images/map_images/deco/deco_6.bmp"); break;
                case 7: deco_pixmap.load(":/images/map_images/deco/deco_7.bmp"); break;
                }

                QGraphicsPixmapItem* pix_item = scene.addPixmap(deco_pixmap);

                pix_item->setOffset(col * image_size, row * image_size);
                pix_item->setZValue(0);
                pix_item->setVisible(true);
                map_images.append(pix_item);
            }
            else
            {
                QGraphicsPixmapItem* pix_item = scene.addPixmap(QPixmap(":/images/map_images/path.bmp"));
                pix_item->setOffset(col * image_size, row * image_size);
                pix_item->setZValue(0);
                pix_item->setVisible(true);
                map_images.append(pix_item);
            }

            QGraphicsRectItem* rect = scene.addRect(col * image_size,row * image_size, image_size, image_size);
            rect -> setVisible(false);
            rect -> setZValue(10);
            map_indicators.append(rect);
        }
    }
}

//Render tower GUI
void GameMap::render_tower_gui(QGraphicsScene &scene, Player* player)
{
    for(int i = 0; i < tower_images.size(); ++i) {
        scene.removeItem(tower_images[i]);
    }
    tower_images.clear();
    for (int idx = 0; idx < player -> get_num_tower(); ++idx) //Scanning through all towers
    {
        int row = player -> get_towers()[idx] -> get_tower_row();
        int col = player -> get_towers()[idx] -> get_tower_col();

        // get_tower_pixmap_small is a virtual function in the Tower class, so it is more generic
        QGraphicsPixmapItem* pix_item = scene.addPixmap(player->get_towers()[idx]->get_tower_pixmap());
        pix_item->setOffset(col * image_size, row * image_size);
        pix_item->setZValue(3);
        tower_images.append(pix_item);
    }
}

//Rendering enemy GUI
void GameMap::render_enemy_gui(QGraphicsScene &scene, Enemy *enemies_on_map)
{
    int row = enemies_on_map -> get_position_row();
    int col = enemies_on_map -> get_position_col();
    // Render enemy GUI
    QGraphicsPixmapItem* pix_item = scene.addPixmap(enemies_on_map->get_enemy_pixmap());
    pix_item->setOffset(col * image_size, row * image_size);
    pix_item->setZValue(5);
    enemies_images.append(pix_item);
    enemies_on_map -> image = pix_item;

    // (NEW) Render health bars GUI
    QGraphicsPixmapItem* health_bar_item = scene.addPixmap(QPixmap(":/images/health_bars/100.png"));
    health_bar_item->setOffset(col * image_size, row * image_size - 5);
    health_bar_item->setZValue(8);
    health_images.append(health_bar_item);
}

// (NEW) Return a QPixmap of the health bar depending on enemy's health
QPixmap GameMap::get_health_pixmap(Enemy *enemy) {
    double temp = ((static_cast<double>(enemy->get_health())/static_cast<double>(enemy->get_max_health())) * 100);
    int enemy_health_percentage = floor(temp);
//    qDebug() << "enemy max health: " << enemy->get_max_health();
//    qDebug() << "enemy current health: " << enemy->get_health();
    switch(enemy_health_percentage) {
    default: {
        return QPixmap(":/images/health_bars/10.png");
        break;
    }
    case 0 ... 10: {
        return QPixmap(":/images/health_bars/10.png");
        break;
    }
    case 11 ... 20: {
        return QPixmap(":/images/health_bars/20.png");
        break;
    }
    case 21 ... 30: {
        return QPixmap(":/images/health_bars/30.png");
        break;
    }
    case 31 ... 40: {
        return QPixmap(":/images/health_bars/40.png");
        break;
    }
    case 41 ... 50: {
        return QPixmap(":/images/health_bars/50.png");
        break;
    }
    case 51 ... 60: {
        return QPixmap(":/images/health_bars/60.png");
        break;
    }
    case 61 ... 70: {
        return QPixmap(":/images/health_bars/70.png");
        break;
    }
    case 71 ... 80: {
        return QPixmap(":/images/health_bars/80.png");
        break;
    }
    case 81 ... 90: {
        return QPixmap(":/images/health_bars/90.png");
        break;
    }
    case 91 ... 100: {
        return QPixmap(":/images/health_bars/100.png");
        break;
    }
    }
}

//Rendering GUI in real time
void GameMap::render_enemy_movement(Enemy *enemies_on_map, int idx) //enemies_on_map = QList<Enemy> enemies_on_map[idx], and correlate with QList<QGraphicsPixmapItem*> enemies_images[idx]
{
    int current_row = enemies_on_map -> get_position_row();
    int current_col = enemies_on_map -> get_position_col();

    //Assuming the path direction will not lead to out-of-bound situation
    if (terrain_map[current_row * num_cols + current_col] == TerrainState::UP)
    {
        enemies_images[idx] -> setOffset(current_col*image_size, current_row*image_size - enemies_on_map -> current_pixel);

        // (NEW) Added health bar status
        health_images[idx]->setOffset(current_col*image_size, current_row*image_size - (enemies_on_map -> current_pixel) - 5);
        health_images[idx]->setPixmap(get_health_pixmap(enemies_on_map));

        if (enemies_on_map -> current_pixel == image_size) //When the enemy moved numbers of pixels of its size, move its game grid coordinates
        {
            update_tower_and_enemies_map(enemies_on_map -> get_position_row(), enemies_on_map -> get_position_col(), EnemiesTowersState::SPACE);
            (enemies_on_map -> position_row)--;
            update_tower_and_enemies_map(enemies_on_map -> get_position_row(), enemies_on_map -> get_position_col(), EnemiesTowersState::ENEMY);
            enemies_on_map -> current_pixel = 0;
        }
        ++(enemies_on_map -> current_pixel);
    }
    else if (terrain_map[current_row * num_cols + current_col] == TerrainState::DOWN)
    {
        enemies_images[idx] -> setOffset(current_col*image_size, current_row*image_size + enemies_on_map -> current_pixel);

        // (NEW) Added health bar status
        health_images[idx]->setOffset(current_col*image_size, current_row*image_size + (enemies_on_map -> current_pixel) - 5);
        health_images[idx]->setPixmap(get_health_pixmap(enemies_on_map));

        if (enemies_on_map -> current_pixel == image_size) //When the enemy moved numbers of pixels of its size, move its game grid coordinates
        {
            update_tower_and_enemies_map(enemies_on_map -> get_position_row(), enemies_on_map -> get_position_col(), EnemiesTowersState::SPACE);
            (enemies_on_map -> position_row)++;
            update_tower_and_enemies_map(enemies_on_map -> get_position_row(), enemies_on_map -> get_position_col(), EnemiesTowersState::ENEMY);
            enemies_on_map -> current_pixel = 0;
        }
        ++(enemies_on_map -> current_pixel);
    }
    else if (terrain_map[current_row * num_cols + current_col] == TerrainState::LEFT)
    {
        enemies_images[idx] -> setOffset(current_col*image_size - enemies_on_map -> current_pixel, current_row*image_size );

        // (NEW) Added health bar status
        health_images[idx]->setOffset(current_col*image_size - enemies_on_map -> current_pixel, current_row*image_size - 5);
        health_images[idx]->setPixmap(get_health_pixmap(enemies_on_map));

        if (enemies_on_map -> current_pixel == image_size) //When the enemy moved numbers of pixels of its size, move its game grid coordinates
        {
            update_tower_and_enemies_map(enemies_on_map -> get_position_row(), enemies_on_map -> get_position_col(), EnemiesTowersState::SPACE);
            (enemies_on_map -> position_col)--;
            update_tower_and_enemies_map(enemies_on_map -> get_position_row(), enemies_on_map -> get_position_col(), EnemiesTowersState::ENEMY);
            enemies_on_map -> current_pixel = 0;
        }
        ++(enemies_on_map -> current_pixel);
    }
    else if (terrain_map[current_row * num_cols + current_col] == TerrainState::RIGHT)
    {
        enemies_images[idx] -> setOffset(current_col*image_size + enemies_on_map -> current_pixel, current_row*image_size );

        // (NEW) Added health bar status
        health_images[idx]->setOffset(current_col*image_size + enemies_on_map -> current_pixel, current_row*image_size - 5);
        health_images[idx]->setPixmap(get_health_pixmap(enemies_on_map));

        if (enemies_on_map -> current_pixel == image_size) //When the enemy moved numbers of pixels of its size, move its game grid coordinates
        {
            update_tower_and_enemies_map(enemies_on_map -> get_position_row(), enemies_on_map -> get_position_col(), EnemiesTowersState::SPACE);
            (enemies_on_map -> position_col)++;
            update_tower_and_enemies_map(enemies_on_map -> get_position_row(), enemies_on_map -> get_position_col(), EnemiesTowersState::ENEMY);
            enemies_on_map -> current_pixel = 0;
        }
        ++(enemies_on_map -> current_pixel);
    }
}

bool GameMap::build_on_terrain(int row, int col) {
    // The selected row and col is not available for building tower
    if(terrain_map[row * num_cols + col] != TerrainState::SPACE) return false;

    terrain_map[row * num_cols + col] = TerrainState::BLOCK;
    tower_and_enemies_map[row * num_cols + col] = EnemiesTowersState::TOWER;
    return true;
}

void GameMap::update_terrain_map(int row, int col, TerrainState state)
{
    terrain_map[row * num_cols + col] = state;
}

void GameMap::update_tower_and_enemies_map(int row, int col, EnemiesTowersState state)
{
    tower_and_enemies_map[row * num_cols + col] = state;
}

void GameMap::reset_indicators() {
    // reset all the indicators
    for(int i = 0; i < (num_rows*num_cols); ++i) {
        map_indicators[i]->setVisible(false);
    }
}

void GameMap::indicate_area(int row, int col, int rad) {
    // Indicate an area given row, col and radius
    for(int i = 0; i < num_rows; ++i) {
        for(int j = 0; j < num_cols; ++j) {
            int delta_row = abs(row - i);
            int delta_col = abs(col - j);
            if(delta_row + delta_col < rad + 1)
                map_indicators[i * num_cols + j]->setVisible(true);
        }
    }
}
