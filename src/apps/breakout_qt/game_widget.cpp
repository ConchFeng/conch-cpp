#include "game_widget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <cmath>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), state_(GameState::READY), score_(0), lives_(3), level_(1),
      leftPressed_(false), rightPressed_(false) {
    
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &GameWidget::gameLoop);
    
    reset();
    timer_->start(16); // ~60 FPS
}

void GameWidget::reset() {
    // Initialize paddle
    paddle_.x = WINDOW_WIDTH / 2.0f;
    paddle_.y = WINDOW_HEIGHT - 40;
    paddle_.width = 100;
    paddle_.height = 15;
    
    // Initialize ball
    ball_.pos.x = paddle_.x;
    ball_.pos.y = paddle_.y - paddle_.height / 2 - ball_.radius - 2;
    ball_.vel.dx = 0;
    ball_.vel.dy = 0;
    ball_.launched = false;
    
    score_ = 0;
    lives_ = 3;
    level_ = 1;
    state_ = GameState::READY;
    
    initLevel(level_);
    update();
}

void GameWidget::initLevel(int level) {
    bricks_.clear();
    
    // Create brick pattern based on level
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            // Skip some bricks for variety in higher levels
            if (level > 1 && (row + col) % (level + 2) == 0) {
                continue;
            }
            
            // Determine brick strength based on row
            int hits = 1;
            if (row < 2) {
                hits = 3; // Top rows are strongest
            } else if (row < 4) {
                hits = 2;
            }
            
            // Increase hits for higher levels
            if (level > 1) {
                hits = std::min(3, hits + (level - 1) / 2);
            }
            
            bricks_.emplace_back(row, col, hits);
        }
    }
}

void GameWidget::launchBall() {
    if (!ball_.launched) {
        ball_.launched = true;
        // Launch at slight angle
        float angle = -1.3f; // Roughly 75 degrees upward
        ball_.vel.dx = BALL_SPEED * std::sin(angle);
        ball_.vel.dy = BALL_SPEED * std::cos(angle);
    }
}

void GameWidget::updateBall() {
    if (!ball_.launched) {
        // Ball follows paddle before launch
        ball_.pos.x = paddle_.x;
        ball_.pos.y = paddle_.y - paddle_.height / 2 - ball_.radius - 2;
        return;
    }
    
    // Update position
    ball_.pos.x += ball_.vel.dx;
    ball_.pos.y += ball_.vel.dy;
    
    // Wall collisions
    if (ball_.pos.x - ball_.radius <= 0 || ball_.pos.x + ball_.radius >= WINDOW_WIDTH) {
        ball_.vel.dx = -ball_.vel.dx;
        ball_.pos.x = std::clamp(ball_.pos.x, ball_.radius, WINDOW_WIDTH - ball_.radius);
    }
    
    if (ball_.pos.y - ball_.radius <= 0) {
        ball_.vel.dy = -ball_.vel.dy;
        ball_.pos.y = ball_.radius;
    }
    
    // Bottom boundary - lose life
    if (ball_.pos.y - ball_.radius > WINDOW_HEIGHT) {
        lives_--;
        if (lives_ <= 0) {
            state_ = GameState::GAME_OVER;
        } else {
            ball_.launched = false;
            ball_.pos.x = paddle_.x;
            ball_.pos.y = paddle_.y - paddle_.height / 2 - ball_.radius - 2;
            ball_.vel.dx = 0;
            ball_.vel.dy = 0;
            state_ = GameState::READY;
        }
    }
}

void GameWidget::checkPaddleCollision() {
    if (!ball_.launched) return;
    
    // Check if ball is near paddle vertically
    if (ball_.pos.y + ball_.radius >= paddle_.y - paddle_.height / 2 &&
        ball_.pos.y + ball_.radius <= paddle_.y + paddle_.height / 2 + 5 &&
        ball_.vel.dy > 0) {
        
        // Check horizontal overlap
        if (ball_.pos.x >= paddle_.x - paddle_.width / 2 &&
            ball_.pos.x <= paddle_.x + paddle_.width / 2) {
            
            // Bounce ball
            ball_.vel.dy = -std::abs(ball_.vel.dy);
            
            // Adjust horizontal velocity based on hit position
            float hitPos = (ball_.pos.x - paddle_.x) / (paddle_.width / 2);
            ball_.vel.dx = hitPos * BALL_SPEED * 0.75f;
            
            // Normalize velocity to maintain speed
            float speed = std::sqrt(ball_.vel.dx * ball_.vel.dx + ball_.vel.dy * ball_.vel.dy);
            ball_.vel.dx = (ball_.vel.dx / speed) * BALL_SPEED;
            ball_.vel.dy = (ball_.vel.dy / speed) * BALL_SPEED;
            
            ball_.pos.y = paddle_.y - paddle_.height / 2 - ball_.radius;
        }
    }
}

void GameWidget::checkBrickCollision() {
    if (!ball_.launched) return;
    
    for (auto& brick : bricks_) {
        if (brick.destroyed) continue;
        
        // Calculate brick boundaries
        float brickX = BRICK_OFFSET_LEFT + brick.col * (BRICK_WIDTH + BRICK_PADDING);
        float brickY = BRICK_OFFSET_TOP + brick.row * (BRICK_HEIGHT + BRICK_PADDING);
        
        // Simple AABB collision detection
        if (ball_.pos.x + ball_.radius >= brickX &&
            ball_.pos.x - ball_.radius <= brickX + BRICK_WIDTH &&
            ball_.pos.y + ball_.radius >= brickY &&
            ball_.pos.y - ball_.radius <= brickY + BRICK_HEIGHT) {
            
            // Damage brick
            brick.hits--;
            if (brick.hits <= 0) {
                brick.destroyed = true;
            }
            
            score_ += getBrickScore(brick.hits + 1);
            
            // Determine bounce direction based on hit position
            float ballCenterX = ball_.pos.x;
            float ballCenterY = ball_.pos.y;
            float brickCenterX = brickX + BRICK_WIDTH / 2;
            float brickCenterY = brickY + BRICK_HEIGHT / 2;
            
            float dx = ballCenterX - brickCenterX;
            float dy = ballCenterY - brickCenterY;
            
            // Bounce based on which side was hit
            if (std::abs(dx / BRICK_WIDTH) > std::abs(dy / BRICK_HEIGHT)) {
                ball_.vel.dx = -ball_.vel.dx;
            } else {
                ball_.vel.dy = -ball_.vel.dy;
            }
            
            // Check if level complete
            bool allDestroyed = true;
            for (const auto& b : bricks_) {
                if (!b.destroyed) {
                    allDestroyed = false;
                    break;
                }
            }
            if (allDestroyed) {
                state_ = GameState::LEVEL_COMPLETE;
            }
            
            break; // Only one collision per frame
        }
    }
}

void GameWidget::checkCollisions() {
    checkPaddleCollision();
    checkBrickCollision();
}

void GameWidget::nextLevel() {
    level_++;
    ball_.launched = false;
    ball_.pos.x = paddle_.x;
    ball_.pos.y = paddle_.y - paddle_.height / 2 - ball_.radius - 2;
    ball_.vel.dx = 0;
    ball_.vel.dy = 0;
    initLevel(level_);
    state_ = GameState::READY;
}

void GameWidget::gameLoop() {
    if (state_ == GameState::PLAYING) {
        // Update paddle based on input
        if (leftPressed_) {
            paddle_.x -= PADDLE_SPEED;
        }
        if (rightPressed_) {
            paddle_.x += PADDLE_SPEED;
        }
        paddle_.x = std::clamp(paddle_.x, paddle_.width / 2, 
                              WINDOW_WIDTH - paddle_.width / 2);
        
        updateBall();
        checkCollisions();
    }
    
    update();
}

QColor GameWidget::getBrickColor(int hits) const {
    switch (hits) {
        case 3: return QColor(220, 20, 60);    // Crimson
        case 2: return QColor(255, 165, 0);    // Orange
        case 1: return QColor(50, 205, 50);    // Lime Green
        default: return Qt::gray;
    }
}

int GameWidget::getBrickScore(int hits) const {
    return hits * 10;
}

void GameWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw background
    painter.fillRect(0, 0, width(), height(), QColor(20, 20, 30));
    
    // Draw bricks
    for (const auto& brick : bricks_) {
        if (brick.destroyed) continue;
        
        float x = BRICK_OFFSET_LEFT + brick.col * (BRICK_WIDTH + BRICK_PADDING);
        float y = BRICK_OFFSET_TOP + brick.row * (BRICK_HEIGHT + BRICK_PADDING);
        
        QColor color = getBrickColor(brick.hits);
        painter.fillRect(QRectF(x, y, BRICK_WIDTH, BRICK_HEIGHT), color);
        
        // Draw brick border
        painter.setPen(QPen(QColor(255, 255, 255, 100), 2));
        painter.drawRect(QRectF(x, y, BRICK_WIDTH, BRICK_HEIGHT));
    }
    
    // Draw paddle
    painter.fillRect(QRectF(paddle_.x - paddle_.width / 2, 
                           paddle_.y - paddle_.height / 2,
                           paddle_.width, paddle_.height), 
                    QColor(100, 149, 237)); // Cornflower Blue
    
    // Draw ball
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(ball_.pos.x, ball_.pos.y), 
                       ball_.radius, ball_.radius);
    
    // Draw HUD
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(10, 30, QString("Score: %1").arg(score_));
    painter.drawText(200, 30, QString("Lives: %1").arg(lives_));
    painter.drawText(350, 30, QString("Level: %1").arg(level_));
    painter.drawText(550, 30, "Controls: Arrow Keys / Mouse");
    
    // Draw state messages
    if (state_ == GameState::READY) {
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, 
                        "Press SPACE to launch ball");
    } else if (state_ == GameState::PAUSED) {
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.fillRect(rect(), QColor(0, 0, 0, 150));
        painter.drawText(rect(), Qt::AlignCenter, "PAUSED");
    } else if (state_ == GameState::GAME_OVER) {
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.fillRect(rect(), QColor(0, 0, 0, 180));
        painter.drawText(rect(), Qt::AlignCenter, 
                        QString("GAME OVER\nScore: %1\nPress R to restart").arg(score_));
    } else if (state_ == GameState::LEVEL_COMPLETE) {
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.fillRect(rect(), QColor(0, 100, 0, 180));
        painter.drawText(rect(), Qt::AlignCenter, 
                        QString("LEVEL %1 COMPLETE!\nPress ENTER for next level").arg(level_));
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_A:
            leftPressed_ = true;
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            rightPressed_ = true;
            break;
        case Qt::Key_Space:
            if (state_ == GameState::READY) {
                launchBall();
                state_ = GameState::PLAYING;
            } else if (state_ == GameState::PLAYING) {
                state_ = GameState::PAUSED;
            } else if (state_ == GameState::PAUSED) {
                state_ = GameState::PLAYING;
            }
            break;
        case Qt::Key_R:
            reset();
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            if (state_ == GameState::LEVEL_COMPLETE) {
                nextLevel();
            }
            break;
    }
}

void GameWidget::mouseMoveEvent(QMouseEvent *event) {
    if (state_ == GameState::PLAYING || state_ == GameState::READY) {
        paddle_.x = std::clamp(static_cast<float>(event->pos().x()), 
                              paddle_.width / 2, 
                              WINDOW_WIDTH - paddle_.width / 2);
    }
}
