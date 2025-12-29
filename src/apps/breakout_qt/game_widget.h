#pragma once
#include <QWidget>
#include <QTimer>
#include <deque>
#include <random>
#include <vector>
#include "game_types.h"


class GameWidget : public QWidget {
    Q_OBJECT
    
public:
    explicit GameWidget(QWidget *parent = nullptr);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    
private slots:
    void gameLoop();
    
private:
    void reset();
    void initLevel(int level);
    void launchBall();
    void updateBall();
    void checkCollisions();
    void checkBrickCollision();
    void checkPaddleCollision();
    void nextLevel();
    
    QColor getBrickColor(int hits) const;
    int getBrickScore(int hits) const;
    
    // Game constants
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr int BRICK_ROWS = 8;
    static constexpr int BRICK_COLS = 10;
    static constexpr int BRICK_WIDTH = 70;
    static constexpr int BRICK_HEIGHT = 20;
    static constexpr int BRICK_PADDING = 5;
    static constexpr int BRICK_OFFSET_TOP = 60;
    static constexpr int BRICK_OFFSET_LEFT = 35;
    static constexpr float BALL_SPEED = 5.0f;
    static constexpr float PADDLE_SPEED = 8.0f;
    
    // Game state
    GameState state_;
    Paddle paddle_;
    Ball ball_;
    std::vector<Brick> bricks_;
    int score_;
    int lives_;
    int level_;
    
    // Input state
    bool leftPressed_;
    bool rightPressed_;
    
    QTimer *timer_;
};
