#include <fstream>

#include <QFileDialog>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QTimer>
#include <QSignalMapper>
#include <string>



#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clickableview.h"
#include "myMessagebox.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    srand (time(NULL));

    // Link the scene with the view
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->show();

    // Link the Wave scene with the view
    ui->graphicsWave->setScene(&wave_scene);
    wavePixmap = wave_scene.addPixmap(QPixmap(":/images/wave_images/wave1.png"));
    wavePixmap->setVisible(true);
    ui->graphicsWave->show();

    // Link the Gold scene with the view
    ui->graphicsGold->setScene(&gold_scene);
    goldPixmap = gold_scene.addPixmap(QPixmap(":/images/gold_images/gold.png"));
    goldPixmap->setVisible(true);
    ui->graphicsGold->show();

    // Link the Shop scene with the view
    ui->graphicsShop->setScene(&shop_scene);
    shopPixmap = shop_scene.addPixmap(QPixmap(":/images/shop_images/shop.png"));
    shopPixmap->setVisible(true);
    ui->graphicsShop->show();

    //==========================
    // Set the background images for stats view
    // Frame
    ui->graphicsStats->setScene(&stats_scene);
    QGraphicsPixmapItem* stats_frame = stats_scene.addPixmap(QPixmap(":/images/stats_images/stats_frame.png"));
    stats_frame->setVisible(true);

    // Tower Displayed
    stats_displayed_tower = stats_scene.addPixmap(QPixmap(""));
    stats_displayed_tower->setOffset(175, 72);
    stats_displayed_tower->setVisible(false);

    ui->graphicsStats->show();

    // Initialize the player
    player = new Player;

    //==========================
    // Stats view initialization
    stats_view_initialization();
    //==========================

    game_map.render_map_gui(scene);


    // Initialize the money LCD count
    ui->moneyLcdcount->display(player->get_money());

    // connect the signal of the click event in the upper panel and the handler in this class
    connect(ui->graphicsView, &ClickableView::mouseClicked, this, &MainWindow::map_clicked);

    // set button to be visible
    ui->startButton->setEnabled(true);
    ui->upgradeButton->setEnabled(true);
    ui->destroyButton->setEnabled(true);
    ui->confirmButton->setEnabled(true);
    ui->cancelButton->setEnabled(true);


    // Set confirm and cancel button to invisible at start
    ui->confirmButton->setVisible(false);
    ui->cancelButton->setVisible(false);

    // Set upgrade/destroy and cancel button to invisible at start
    ui->upgradeButton->setVisible(false);
    ui->destroyButton->setVisible(false);

    // Set SHOP towers button to visible at start
    ui->buyarrowButton->setVisible(true);
    ui->buycannonButton->setVisible(true);
    ui->buywizardButton->setVisible(true);

    // SHOP: Set button icons to tower images
    ui->buyarrowButton->setIcon(QPixmap(":/images/tower_images/Arrow/Arrow_1.png"));
    ui->buyarrowButton->setIconSize(QSize(41, 41));
    ui->buycannonButton->setIcon(QPixmap(":/images/tower_images/Cannon/Cannon_1.png"));
    ui->buycannonButton->setIconSize(QSize(41, 41));
    ui->buywizardButton->setIcon(QPixmap(":/images/tower_images/Wizard/Wizard_1.png"));
    ui->buywizardButton->setIconSize(QSize(41, 41));

    ui->graphicsView->fitInView(0, 0, 36, 36, Qt::KeepAspectRatio);

    // Button for shopping
    set_buytower_button_enabled(true);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete player;
}

// Stats view initialization
void MainWindow::stats_view_initialization() {
    stats_text.clear();
    QFont big_font = QFont("MV Boli", 22);
    QFont font = QFont("MV Boli", 16);

    int text_position[6][2] = {
        {30, 100},
        {30, 140},
        {30, 180},
        {30, 220},
        {30, 260},
        {30, 300}
    };

    QGraphicsTextItem* _tower_stats = new QGraphicsTextItem;
    _tower_stats->setPos(40, 20);
    _tower_stats->setFont(big_font);
    stats_scene.addItem(_tower_stats);
    stats_text.append(_tower_stats);

    // 6 items to be added for display in the stats view
    for(int i = 0; i < 6; ++i) {
        QGraphicsTextItem* temp = new QGraphicsTextItem;
        temp->setPos(text_position[i][0], text_position[i][1]);
        temp->setFont(font);
        stats_scene.addItem(temp);
        stats_text.append(temp);
    }

    // update the text in the display window
    update_stats_view();

    // Extra: Health display initialization
    QGraphicsTextItem* _health_display = new QGraphicsTextItem;
    _health_display->setPos(30, 360);
    _health_display->setFont(font);
    QString temp_text = QString("Health: " + QString(QString::number(player->get_max_health())) + "/" + QString(QString::number(player->get_max_health())));
    _health_display->setPlainText(temp_text);
    stats_scene.addItem(_health_display);
    health_display = _health_display;
}

void MainWindow::update_stats_view() {
    // If no tower selected, set all to empty
    if(selected_tower == -1) {
        stats_text[MainWindow::TOWER_STATS]->setPlainText("Type: ");
        stats_text[MainWindow::LEVEL]->setPlainText("Level: ");
        stats_text[MainWindow::DAMAGE]->setPlainText("Damage: ");
        stats_text[MainWindow::ATTACK_RANGE]->setPlainText("Att. Range: ");
        stats_text[MainWindow::ATTACK_SPEED]->setPlainText("Att. Speed: ");
        stats_text[MainWindow::COST]->setPlainText("Cost: ");
        stats_displayed_tower->setVisible(false);
    }
    // selected a tower, show the tower's stats
    else {
        Tower* _tower = player->get_towers()[selected_tower];

        // Type: stats_text[1]
        QString _type("Type: " + _tower->tower_name_string());
        stats_displayed_tower->setPixmap(_tower->get_tower_pixmap());
        stats_displayed_tower->setVisible(true);
        stats_text[MainWindow::TOWER_STATS]->setPlainText(_type);


        // Level: stats_text[2]
        QString _level = QString("Level: " + QString(QString::number(_tower->get_tower_level() + 1)));
        stats_text[MainWindow::LEVEL]->setPlainText(_level);

        // Damage: stats_text[3]
        QString _damage = QString("Damage: " + QString(QString::number(_tower->get_tower_damage())));
        stats_text[MainWindow::DAMAGE]->setPlainText(_damage);

        // Range: stats_text[4]
        QString _attack_range = QString("Att. Range: " + QString(QString::number(_tower->get_tower_range())));
        stats_text[MainWindow::ATTACK_RANGE]->setPlainText(_attack_range);

        // Attack Speed: stats_text[5]
        QString _attack_speed = QString("Att. Speed: " + QString(QString::number(static_cast<double>(_tower->get_tower_attack_speed())/1000)) + " s");
        stats_text[MainWindow::ATTACK_SPEED]->setPlainText(_attack_speed);

        // Cost: stats_text[6]
        QString _cost = QString("Cost: " + QString(QString::number(_tower->get_tower_cost())));
        stats_text[MainWindow::COST]->setPlainText(_cost);
    }
}

void MainWindow::update_health_display() {
    QString _level = QString("Health: " + QString(QString::number(player->get_health())) + "/" + QString(QString::number(player->get_max_health())));
    health_display->setPlainText(_level);
}

//Render enemy GUI

void MainWindow::render_gui(int idx)
{
    game_map.render_enemy_movement(enemies_on_map[idx], idx);
}

void MainWindow::start_rendering_enemies_gui()
{
    if (enemies_clock.isEmpty())
    {}
    else if (enemies_clock[0] != nullptr)
    {
        for (int idx = 0; idx < enemies_clock.size(); ++idx)
        {
            enemies_clock[idx] -> stop();
        }
    }

    enemies_clock.clear(); //Reset all the clock
    for (int idx = 0; idx < enemies_on_map.size(); ++idx)
    {

        QTimer* new_timer = new QTimer(this);
        QSignalMapper* signalmapper = new QSignalMapper(this);
        connect(new_timer, SIGNAL(timeout()), signalmapper, SLOT(map()));
        signalmapper -> setMapping(new_timer, idx);
        connect(signalmapper, SIGNAL(mapped(int)), this, SLOT(render_gui(int)));
        new_timer -> start((enemies_on_map[idx] -> get_speed())); // enemy speed: 100 = 1 block(45pixels) per second
        enemies_clock.append(new_timer);
    }
}

void MainWindow::spawn_enemy()
{
    if (total_enemies.isEmpty())
        return;
    enemies_on_map.append(total_enemies[0]);
    game_map.render_enemy_gui(scene, total_enemies[0]);
    total_enemies.removeFirst();
    start_rendering_enemies_gui();
}

Enemy* MainWindow::generate_random_enemies(int idx)
{
    //40% spawn Scout, 40% spawn Armour, 20% spawn WarMachine
    int random = rand() % 100; // A integer in 0-99
    if (0 <= random && random < 40)
    {
        Enemy* new_enemy = new Scout(idx, game_map.spawn_row, game_map.spawn_col, current_wave);
        return new_enemy;
    }
    else if (40 <= random && random < 80)
    {
        Enemy* new_enemy = new Armour(idx, game_map.spawn_row, game_map.spawn_col, current_wave);
        return new_enemy;
    }
    else
    {
        Enemy* new_enemy = new WarMachine(idx, game_map.spawn_row, game_map.spawn_col, current_wave);
        return new_enemy;
    }
}

void MainWindow::on_startButton_released()
{
    //delete game_clock; //delete the previous wave clock

    selected_tower = -1;
    update_stats_view();

    round_over = false;
    // load map and start button
    ui->startButton->setEnabled(false);

    // Menu for when selected a tower
    ui->upgradeButton->setVisible(false);
    ui->destroyButton->setVisible(false);
    game_map.reset_indicators();
    update_stats_view();

    // Menu for Shop
    set_buytower_button_enabled(false);

    //Start the timer for this wave
    game_clock = new QTimer(this);
    connect(game_clock, SIGNAL(timeout()), this, SLOT(count_down()));
    game_clock -> start(50); //Call count_down() function per second

    //Start spawning enemy
    for (int idx = 0; idx < (current_wave + 1) * 10; ++idx)
    {
        // create a new enemy, with the level being the current_wave
        total_enemies.append(generate_random_enemies(idx));

    }

    //Spawn clock use for loop and single shot timer

    spawn_clock = new QTimer(this);
    connect(spawn_clock, SIGNAL(timeout()), this, SLOT(spawn_enemy()));
    spawn_clock -> start(2000 - current_wave * 150);


}

void MainWindow::on_upgradeButton_released()
{
    // If no tower is selected, do nothing
    if(selected_tower == -1) return;

    if(player->get_money() < player->get_towers()[selected_tower]->get_tower_upgrade_cost()) {
        myMessagebox("No Money!", "You need $" + QString(QString::number(player->get_towers()[selected_tower]->get_tower_upgrade_cost())) +
                                                         " to upgrade!");
        return;
    }

    // Level up the selected tower
    if(player->get_towers()[selected_tower]->tower_level_up()) {
        player->set_money(player->get_money() - player->get_towers()[selected_tower]->get_tower_upgrade_cost());
        ui->moneyLcdcount->display(player->get_money());
    }
    Tower* _tower = player->get_towers()[selected_tower];

    update_stats_view();
    game_map.render_tower_gui(scene, player);
    game_map.reset_indicators();
    game_map.indicate_area(_tower->get_tower_row(), _tower->get_tower_col(), _tower->get_tower_range());
}

void MainWindow::on_destroyButton_released()
{
    // If no tower is selected, do nothing
    if(selected_tower == -1) return;

    // Get the tower that are to be destroyed
    Tower* _tower = player->get_towers()[selected_tower];

    // Get the money to be received
    int receive_gold = _tower->get_tower_cost();

    // Update the terrain state
    game_map.update_terrain_map(_tower->get_tower_row(), _tower->get_tower_col(), GameMap::TerrainState::SPACE);

    // Destroy the tower
    player->destroy_tower(_tower);

    // Rerender all the towers
    game_map.render_tower_gui(scene, player);

    // Add money to player
    player->set_money(player->get_money() + receive_gold);
    ui->moneyLcdcount->display(player->get_money());

    selected_tower = -1;
    ui->upgradeButton->setVisible(false);
    ui->destroyButton->setVisible(false);
    game_map.reset_indicators();
    update_stats_view();
    set_buytower_button_enabled(true);
}

void MainWindow::on_buyarrowButton_released()
{
    if(player->get_num_tower() >= player->get_max_tower()) {
        myMessagebox("Cannot build anymore", "You have reached the tower limit: " + QString(QString::number(player->get_max_tower())));
        return;
    }

    // If you have selected a tower, reset everything
    if(selected_tower != -1) {
        ui->upgradeButton->setVisible(false);
        ui->destroyButton->setVisible(false);
        selected_tower = -1;
        update_stats_view();
    }
    if(player->get_money() < 10) {
        myMessagebox("No Money!", "You need $10 to build this tower!");
        return;
    }
    is_placing_tower = true;
    placing_tower_type = Tower::TowerType::ARROW;

    ui->startButton->setEnabled(false);

    set_buytower_button_enabled(false);
}

void MainWindow::on_buycannonButton_released()
{
    if(player->get_num_tower() >= player->get_max_tower()) {
        myMessagebox("Cannot build anymore", "You have reached the tower limit: " + QString(QString::number(player->get_max_tower())));
        return;
    }

    // If you have selected a tower, reset everything
    if(selected_tower != -1) {
        ui->upgradeButton->setVisible(false);
        ui->destroyButton->setVisible(false);
        selected_tower = -1;
        update_stats_view();
    }
    if(player->get_money() < 10) {
        myMessagebox("No Money!", "You need $10 to build this tower!");
        return;
    }
    is_placing_tower = true;
    placing_tower_type = Tower::TowerType::CANNON;

    ui->startButton->setEnabled(false);

    set_buytower_button_enabled(false);
}

void MainWindow::on_buywizardButton_released()
{
    if(player->get_num_tower() >= player->get_max_tower()) {
        myMessagebox("Cannot build anymore", "You have reached the tower limit: " + QString(QString::number(player->get_max_tower())));
        return;
    }

    // If you have selected a tower, reset everything
    if(selected_tower != -1) {
        ui->upgradeButton->setVisible(false);
        ui->destroyButton->setVisible(false);
        selected_tower = -1;
        update_stats_view();
    }
    if(player->get_money() < 10) {
        myMessagebox("No Money!", "You need $10 to build this tower!");
        return;
    }
    is_placing_tower = true;
    placing_tower_type = Tower::TowerType::WIZARD;

    ui->startButton->setEnabled(false);

    set_buytower_button_enabled(false);
}

void MainWindow::on_confirmButton_released()
{
    // If the user is not confirming to build a tower, do nothing
    if(confirming_tower == -1) return;

    // Reduce player's money
    player->set_money(player->get_money() - player->get_towers()[confirming_tower]->get_tower_cost());
    ui->moneyLcdcount->display(player->get_money());

    // Display new money count
    ui->moneyLcdcount->display(player->get_money());

    // Reset value
    confirming_tower = -1;

    // Reset map indicators (for attack range)
    game_map.reset_indicators();

    // hide buttons that should not be pressed now
    ui->startButton->setEnabled(true);

    // Button for Upgrading/Destroying
    ui->upgradeButton->setVisible(false);
    ui->destroyButton->setVisible(false);
    update_stats_view();

    // Button for shopping
    set_buytower_button_enabled(true);

    // Button for Confirm building
    ui->confirmButton->setVisible(false);
    ui->cancelButton->setVisible(false);
}

void MainWindow::on_cancelButton_released()
{
    // If the user is not confirming to build a tower, do nothing
    if(confirming_tower == -1) return;

    // Get the tower that are to be destroyed
    Tower* _tower = player->get_towers()[confirming_tower];

    // Update the terrain state
    game_map.update_terrain_map(_tower->get_tower_row(), _tower->get_tower_col(), GameMap::TerrainState::SPACE);

    // Destroy the tower
    player->destroy_tower(_tower);

    // Rerender all the towers
    game_map.render_tower_gui(scene, player);

    selected_tower = -1;
    confirming_tower = -1;

    // Reset map indicators (for attack range)
    game_map.reset_indicators();

    // hide buttons that should not be pressed now
    ui->startButton->setEnabled(true);
    // Button for Upgrading/Destroying
    ui->upgradeButton->setVisible(false);
    ui->destroyButton->setVisible(false);
    update_stats_view();
    // Button for shopping
    set_buytower_button_enabled(true);
    // Button for Confirm building
    ui->confirmButton->setVisible(false);
    ui->cancelButton->setVisible(false);
}

void MainWindow::set_buytower_button_enabled(bool status) {
    ui->buyarrowButton->setEnabled(status);
    ui->buycannonButton->setEnabled(status);
    ui->buywizardButton->setEnabled(status);
}

// Display the game end window
void MainWindow::game_end(bool win) {
    for (int idx = 0; idx < enemies_clock.size(); ++idx) {
        enemies_clock[idx] -> stop();
    }
    if(win) {
        myMessagebox("Game End!", "Congratulations! You Won! :)))");
    } else {
        myMessagebox("Game End!", "You lose!! :(((");
    }
}

void MainWindow::update_wave_pixmap() {
    switch(current_wave) {
    case 0: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave1.png"));
        break;
    }
    case 1: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave2.png"));
        break;
    }
    case 2: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave3.png"));
        break;
    }
    case 3: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave4.png"));
        break;
    }
    case 4: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave5.png"));
        break;
    }
    case 5: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave6.png"));
        break;
    }
    case 6: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave7.png"));
        break;
    }
    case 7: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave8.png"));
        break;
    }
    case 8: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave9.png"));
        break;
    }
    case 9: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave10.png"));
        break;
    }
    default: {
        wavePixmap->setPixmap(QPixmap(":/images/wave_images/wave10.png"));
        break;
    }
    }
}

// Called after each round(wave) is over
void MainWindow::end_round() {
    round_over = true;

    //Clear clocks
    player -> reset_focus();
    enemies_clock.clear();
    enemies_on_map.clear();
    total_enemies.clear();
    delete spawn_clock;
    spawn_clock = nullptr;
    game_clock -> stop();
    delete game_clock;

    // Game End
    if (current_wave == max_wave)
    {
        game_end(true);
        return;
    }

    // Go to next wave
    ++current_wave;
    update_wave_pixmap();

    selected_tower = -1;
    ui->upgradeButton->setVisible(false);
    ui->destroyButton->setVisible(false);
    game_map.reset_indicators();
    update_stats_view();
    set_buytower_button_enabled(true);

    // allow the user to start the next round(wave)
    ui->startButton->setEnabled(true);

    // allow the user to shop for tower
    set_buytower_button_enabled(true);


    //Clear GUI
    game_map.enemies_images.clear();
    game_map.health_images.clear();
}

// Called when user click on the map
void MainWindow::map_clicked(int row, int col) {
    // Clicked out of range row/col
    if(row < 0 || row > game_map.get_num_rows() - 1 || col < 0 || col > game_map.get_num_cols()) return;

    // The user is placing a tower
    if(is_placing_tower) {
        // Place the tower on map, going into confirm stage
        if(game_map.build_on_terrain(row, col)) {
            player->place_tower(row, col, placing_tower_type);
            game_map.render_tower_gui(scene, player);

            is_placing_tower = false;

            // Change the confirming_tower value to the tower that are to be confirmed
            get_tower_by_pos(row, col, confirming_tower);

            game_map.indicate_area(row, col, player->get_towers()[confirming_tower]->get_tower_range());

            ui->startButton->setEnabled(false);

            set_buytower_button_enabled(false);

            ui->confirmButton->setVisible(true);
            ui->cancelButton->setVisible(true);
        }
        return;
    }

    // The user is confirming to build a tower, do nothing
    if(confirming_tower != -1) {
        return;
    }

    // If you have already selected a tower before, reset everything first
    if(selected_tower != -1) {
        ui->upgradeButton->setVisible(false);
        ui->destroyButton->setVisible(false);
        game_map.reset_indicators();

        set_buytower_button_enabled(false);
        selected_tower = -1;
        update_stats_view();
    }

    // If the round is not over
    if(!round_over) {
        get_tower_by_pos(row, col, selected_tower);
        if(selected_tower != -1) {
            game_map.indicate_area(row, col, player->get_towers()[selected_tower]->get_tower_range());
            update_stats_view();
        }
    }

    // If the round is over
    else {
        // Get the index of tower clicked on the map
        get_tower_by_pos(row, col, selected_tower);

        // No tower is being clicked
        if(selected_tower == -1) {
            ui->upgradeButton->setVisible(false);
            ui->destroyButton->setVisible(false);
            game_map.reset_indicators();
            update_stats_view();
            set_buytower_button_enabled(true);
            return;
        }

        // A tower is selected, allow users to upgrade/destroy
        ui->upgradeButton->setVisible(true);
        ui->destroyButton->setVisible(true);
        game_map.indicate_area(row, col, player->get_towers()[selected_tower]->get_tower_range());
        update_stats_view();
        set_buytower_button_enabled(false);
    }
}


// get the index of the tower from Row and Col
void MainWindow::get_tower_by_pos(int row, int col, int &tower_index_out) {
    tower_index_out = -1;
    for(int i = 0; i < player->get_num_tower(); ++i) {
        if(player->get_towers()[i]->get_tower_row() == row && player->get_towers()[i]->get_tower_col() == col) {
            tower_index_out = i;
        }
    }
}

bool MainWindow::check_enemy_in_range(Enemy* enemy, Tower* tower) const
{
    int range = tower -> get_tower_range();
    int tower_row = tower -> get_tower_row();
    int tower_col = tower -> get_tower_col();
    int enemy_row = enemy -> get_position_row();
    int enemy_col = enemy -> get_position_col();

    int distance = abs(tower_row - enemy_row) + abs(tower_col - enemy_col);

    if (range >= distance)
        return true;
    else
        return false;
}

bool MainWindow::is_game_over() const {
    if(player->get_health() <= 0) return true;
    return false;
}

void MainWindow::count_down()
{
    bool all_dead = true;
    for (int current_tower_idx = 0; current_tower_idx < player -> get_num_tower(); ++current_tower_idx)
    {
        //Reset current_focus_enemy.clear();
        player -> get_towers()[current_tower_idx] -> current_focus_enemy.clear();
        for (int current_enemy_idx = 0; current_enemy_idx < enemies_on_map.size(); ++current_enemy_idx)
        {
            //If the enemy inside the tower range, append it to the current_focus_enemy list
            //Notice that the list follows the spawn order of the enemies

            if (check_enemy_in_range(enemies_on_map[current_enemy_idx], player -> get_towers()[current_tower_idx]))
                player -> get_towers()[current_tower_idx] -> current_focus_enemy.append(enemies_on_map[current_enemy_idx]);

        }
    }

    //Check enemy reach goal
    //If the enemy is not alive, stop its timer and remove its GUI
    for (int current_enemy_idx = 0; current_enemy_idx < enemies_on_map.size(); ++current_enemy_idx)
    {
        if (!enemies_on_map[current_enemy_idx]->is_alive())
        {
            if (!enemies_on_map[current_enemy_idx] -> rewarded)
            {
                player -> set_money(player -> get_money() + enemies_on_map[current_enemy_idx] -> get_gold());
                ui->moneyLcdcount->display(player->get_money());
                enemies_on_map[current_enemy_idx] -> rewarded = true;
            }
            //Remove GUI
            game_map.enemies_images[current_enemy_idx] -> setVisible(false);
            game_map.health_images[current_enemy_idx] -> setVisible(false);

            //Stop timer
            enemies_clock[current_enemy_idx] -> stop();

            //Check the enemy coordinate to void
            enemies_on_map[current_enemy_idx] -> position_row = -1;
            enemies_on_map[current_enemy_idx] -> position_col = -1;
        }
        else if ((enemies_on_map[current_enemy_idx] -> get_position_row() == game_map.goal_row) && (enemies_on_map[current_enemy_idx] -> get_position_col() == game_map.goal_col)) //Alive enemy reach the goal
        {
             enemies_on_map[current_enemy_idx] -> alive = false;

            //Remove GUI
            game_map.enemies_images[current_enemy_idx] -> setVisible(false);
            game_map.health_images[current_enemy_idx] -> setVisible(false);

            //Stop timer
            enemies_clock[current_enemy_idx] -> stop();

            enemies_on_map[current_enemy_idx] -> position_row = -1;
            enemies_on_map[current_enemy_idx] -> position_col = -1;

            if (enemies_on_map[current_enemy_idx] -> type == Enemy::EnemyType::WARMACHINE)
            {
                player -> health = player -> get_health() - 10;
                if(player->health < 0) player->health = 0;
            }
            else if (enemies_on_map[current_enemy_idx] -> type == Enemy::EnemyType::ARMOUR)
            {
                player -> health = player -> get_health() - 5;
                if(player->health < 0) player->health = 0;
            }
            else
            {
                player -> health = player -> get_health() - 2;
                if(player->health < 0) player->health = 0;
            }

            update_health_display();

            if (is_game_over()) //Lose the game
                game_end(false);
        }
        else
            all_dead = false;
    }

    if (total_enemies.isEmpty() && all_dead)
    {
        end_round();
    }
}
