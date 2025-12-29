#include "mario_game_widget.h"
#include <QKeyEvent>
#include <QPainter>
#include <QDateTime>
#include <QSurfaceFormat>
#include <cmath>
#include <algorithm>

MarioGameWidget::MarioGameWidget(QWidget *parent)
    : QOpenGLWidget(parent), state_(GameState::PLAYING), cameraX_(0), cameraY_(0),
      leftPressed_(false), rightPressed_(false), jumpPressed_(false), downPressed_(false),
      deltaTime_(0.016f), frameCount_(0), fps_(60.0f), gameTime_(0) {
    
    // Request OpenGL 3.3 Core Profile
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    setFormat(format);
    
    setFixedSize(GameConst::VIEWPORT_WIDTH, GameConst::VIEWPORT_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
    
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MarioGameWidget::gameLoop);
    
    lastFrameTime_ = QDateTime::currentMSecsSinceEpoch();
    
    reset();
}

MarioGameWidget::~MarioGameWidget() {
    makeCurrent();
    vbo_.reset();
    vao_.reset();
    shaderProgram_.reset();
    doneCurrent();
}

void MarioGameWidget::reset() {
    // Load level
    level_.loadLevel1();
    
    // Initialize player
    player_ = Player();
    player_.position = level_.getPlayerStart();
    player_.velocity = vec2(0, 0);
    
    // Spawn enemies
    enemies_.clear();
    for (const auto& spawn : level_.getEnemySpawns()) {
        Enemy enemy;
        enemy.type = EntityType::GOOMBA;
        enemy.position = spawn;
        enemy.size = vec2(28, 28);
        enemy.velocity = vec2(-1.5f, 0);
        enemy.movingRight = false;
        enemy.patrolLeft = spawn.x - 100;
        enemy.patrolRight = spawn.x + 100;
        enemies_.push_back(enemy);
    }
    
    // Spawn coins
    coins_.clear();
    for (const auto& coinPos : level_.getCoinPositions()) {
        Entity coin;
        coin.type = EntityType::COIN;
        coin.position = coinPos;
        coin.size = vec2(16, 16);
        coin.animTimer = 0;
        coins_.push_back(coin);
    }
    
    particles_.clear();
    cameraX_ = 0;
    cameraY_ = 0;
    gameTime_ = 0;
    state_ = GameState::PLAYING;
}

void MarioGameWidget::initializeGL() {
    initializeOpenGLFunctions();
    
    glClearColor(0.52f, 0.80f, 0.92f, 1.0f); // Sky blue
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    
    initShaders();
    initBuffers();
    
    timer_->start(16); // ~60 FPS
}

void MarioGameWidget::initShaders() {
    shaderProgram_ = std::make_unique<QOpenGLShaderProgram>();
    
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec4 aColor;
        
        uniform mat4 projection;
        uniform mat4 view;
        
        out vec4 vertexColor;
        
        void main() {
            gl_Position = projection * view * vec4(aPos, 0.0, 1.0);
            vertexColor = aColor;
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec4 vertexColor;
        out vec4 FragColor;
        
        void main() {
            FragColor = vertexColor;
        }
    )";
    
    shaderProgram_->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    shaderProgram_->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    shaderProgram_->link();
}

void MarioGameWidget::initBuffers() {
    vao_ = std::make_unique<QOpenGLVertexArrayObject>();
    vao_->create();
    vao_->bind();
    
    vbo_ = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
    vbo_->create();
    vbo_->bind();
    vbo_->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vbo_->allocate(50000 * sizeof(float));
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    
    vbo_->release();
    vao_->release();
}

void MarioGameWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    
    projection_.setToIdentity();
    projection_.ortho(0.0f, static_cast<float>(GameConst::VIEWPORT_WIDTH), 
                     static_cast<float>(GameConst::VIEWPORT_HEIGHT), 0.0f, -1.0f, 1.0f);
}

void MarioGameWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    shaderProgram_->bind();
    shaderProgram_->setUniformValue("projection", projection_);
    shaderProgram_->setUniformValue("view", view_);
    vao_->bind();
    
    renderTiles();
    renderCoins();
    renderEnemies();
    renderPlayer();
    renderParticles();
    
    vao_->release();
    shaderProgram_->release();
    
    renderHUD();
    
    // FPS counter
    frameCount_++;
    if (frameCount_ >= 30) {
        fps_ = 30.0f / (deltaTime_ * frameCount_);
        frameCount_ = 0;
    }
}

void MarioGameWidget::gameLoop() {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    deltaTime_ = (currentTime - lastFrameTime_) / 1000.0f;
    lastFrameTime_ = currentTime;
    
    if (state_ == GameState::PLAYING) {
        gameTime_ += deltaTime_;
        
        updatePlayer(deltaTime_);
        updateEnemies(deltaTime_);
        updateParticles(deltaTime_);
        checkCollisions();
        updateCamera();
    }
    
    update();
}

void MarioGameWidget::updatePlayer(float deltaTime) {
    // Horizontal movement
    if (leftPressed_ && !rightPressed_) {
        player_.velocity.x -= GameConst::MOVE_ACCELERATION;
        player_.velocity.x = std::max(player_.velocity.x, -GameConst::MAX_MOVE_SPEED);
        player_.facingRight = false;
        player_.state = PlayerState::WALKING;
    } else if (rightPressed_ && !leftPressed_) {
        player_.velocity.x += GameConst::MOVE_ACCELERATION;
        player_.velocity.x = std::min(player_.velocity.x, GameConst::MAX_MOVE_SPEED);
        player_.facingRight = true;
        player_.state = PlayerState::WALKING;
    } else {
        // Deceleration
        player_.velocity.x *= GameConst::MOVE_DECELERATION;
        if (std::abs(player_.velocity.x) < 0.1f) {
            player_.velocity.x = 0;
            if (player_.onGround) {
                player_.state = PlayerState::IDLE;
            }
        }
    }
    
    // Jumping
    if (jumpPressed_ && player_.canJump && player_.onGround) {
        player_.velocity.y = GameConst::JUMP_VELOCITY;
        player_.canJump = false;
        player_.jumpHoldTime = 0;
        player_.state = PlayerState::JUMPING;
    }
    
    // Variable jump height
    if (jumpPressed_ && player_.velocity.y < 0 && player_.jumpHoldTime < 0.3f) {
        player_.jumpHoldTime += deltaTime;
    } else if (!jumpPressed_ && player_.velocity.y < 0) {
        player_.velocity.y *= 0.5f; // Cut jump short
    }
    
    // Apply physics
    PhysicsEngine::applyGravity(player_, deltaTime);
    PhysicsEngine::integrate(player_, deltaTime);
    PhysicsEngine::checkTileCollision(player_, level_, deltaTime);
    
    // Update state
    if (!player_.onGround && player_.velocity.y > 0) {
        player_.state = PlayerState::FALLING;
    }
    
    // Death check (fall off level)
    if (player_.position.y > GameConst::VIEWPORT_HEIGHT + 100) {
        player_.lives--;
        if (player_.lives <= 0) {
            state_ = GameState::GAME_OVER;
        } else {
            player_.position = level_.getPlayerStart();
            player_.velocity = vec2(0, 0);
        }
    }
    
    // Level complete check
    if (player_.position.x > (level_.getWidth() - 10) * GameConst::TILE_SIZE) {
        state_ = GameState::LEVEL_COMPLETE;
    }
}

void MarioGameWidget::updateEnemies(float deltaTime) {
    for (auto& enemy : enemies_) {
        if (!enemy.alive) continue;
        
        // Simple patrol AI
        if (enemy.movingRight) {
            enemy.velocity.x = 1.5f;
            if (enemy.position.x > enemy.patrolRight) {
                enemy.movingRight = false;
            }
        } else {
            enemy.velocity.x = -1.5f;
            if (enemy.position.x < enemy.patrolLeft) {
                enemy.movingRight = true;
            }
        }
        
        // Apply physics
        PhysicsEngine::applyGravity(enemy, deltaTime);
        PhysicsEngine::integrate(enemy, deltaTime);
        PhysicsEngine::checkTileCollision(enemy, level_, deltaTime);
    }
}

void MarioGameWidget::updateParticles(float deltaTime) {
    for (auto& p : particles_) {
        if (p.life <= 0.0f) continue;
        
        p.position += p.velocity;
        p.velocity.y += 0.3f; // Gravity
        p.life -= deltaTime * 2.0f;
    }
    
    particles_.erase(
        std::remove_if(particles_.begin(), particles_.end(),
                      [](const Particle& p) { return p.life <= 0.0f; }),
        particles_.end()
    );
}

void MarioGameWidget::updateCamera() {
    // Camera follows player with dead zone
    float targetX = player_.position.x - GameConst::VIEWPORT_WIDTH / 2.0f + player_.size.x / 2.0f;
    
    // Smooth lerp
    cameraX_ += (targetX - cameraX_) * 0.1f;
    
    // Clamp to level bounds
    cameraX_ = std::max(0.0f, cameraX_);
    cameraX_ = std::min(cameraX_, level_.getWidth() * GameConst::TILE_SIZE - GameConst::VIEWPORT_WIDTH);
    
    // Fixed vertical camera
    cameraY_ = 0;
    
    // Update view matrix
    view_.setToIdentity();
    view_.translate(-cameraX_, -cameraY_);
}

void MarioGameWidget::checkCollisions() {
    // Player vs Enemies
    for (auto& enemy : enemies_) {
        if (!enemy.alive) continue;
        
        if (PhysicsEngine::checkEntityCollision(player_, enemy)) {
            // Check if player is stomping
            if (player_.velocity.y > 0 && player_.position.y + player_.size.y < enemy.position.y + enemy.size.y / 2) {
                // Stomp!
                enemy.alive = false;
                player_.velocity.y = -8.0f; // Bounce
                player_.score += 200;
                spawnParticles(enemy.position + enemy.size * 0.5f, 0.6f, 0.4f, 0.2f);
            } else {
                // Hit! Lose life
                if (player_.isBig) {
                    player_.isBig = false;
                    player_.size = vec2(28, 28);
                } else {
                    player_.lives--;
                    if (player_.lives <= 0) {
                        state_ = GameState::GAME_OVER;
                    } else {
                        player_.position = level_.getPlayerStart();
                        player_.velocity = vec2(0, 0);
                    }
                }
            }
        }
    }
    
    // Player vs Coins
    for (auto& coin : coins_) {
        if (!coin.alive) continue;
        
        if (PhysicsEngine::checkEntityCollision(player_, coin)) {
            coin.alive = false;
            player_.coins++;
            player_.score += 100;
            spawnParticles(coin.position, 1.0f, 0.84f, 0.0f);
        }
    }
}

void MarioGameWidget::spawnParticles(vec2 pos, float r, float g, float b) {
    for (int i = 0; i < 15; ++i) {
        Particle p;
        p.position = pos;
        float angle = (i / 15.0f) * 2.0f * M_PI;
        float speed = 1.5f + (rand() % 100) / 100.0f * 2.0f;
        p.velocity = vec2(std::cos(angle) * speed, std::sin(angle) * speed);
        p.r = r;
        p.g = g;
        p.b = b;
        p.a = 1.0f;
        p.life = 1.0f;
        p.size = 3.0f;
        particles_.push_back(p);
    }
}


// Rendering methods
void MarioGameWidget::renderQuad(vec2 pos, vec2 size, float r, float g, float b, float a) {
    // Transform position to screen space considering camera
    vec2 screenPos = pos;
    
    float vertices[] = {
        screenPos.x,          screenPos.y,          r, g, b, a,
        screenPos.x + size.x, screenPos.y,          r, g, b, a,
        screenPos.x + size.x, screenPos.y + size.y, r, g, b, a,
        
        screenPos.x,          screenPos.y,          r, g, b, a,
        screenPos.x + size.x, screenPos.y + size.y, r, g, b, a,
        screenPos.x,          screenPos.y + size.y, r, g, b, a,
    };
    
    vbo_->bind();
    vbo_->write(0, vertices, sizeof(vertices));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void MarioGameWidget::renderCircle(vec2 pos, float radius, float r, float g, float b, float a) {
    const int segments = 20;
    std::vector<float> vertices;
    vertices.reserve(segments * 3 * 6);
    
    for (int i = 0; i < segments; ++i) {
        float angle1 = (i / static_cast<float>(segments)) * 2.0f * M_PI;
        float angle2 = ((i + 1) / static_cast<float>(segments)) * 2.0f * M_PI;
        
        vertices.insert(vertices.end(), {pos.x, pos.y, r, g, b, a});
        vertices.insert(vertices.end(), {pos.x + radius * std::cos(angle1), pos.y + radius * std::sin(angle1), r, g, b, a});
        vertices.insert(vertices.end(), {pos.x + radius * std::cos(angle2), pos.y + radius * std::sin(angle2), r, g, b, a});
    }
    
    vbo_->bind();
    vbo_->write(0, vertices.data(), vertices.size() * sizeof(float));
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
}

void MarioGameWidget::renderTiles() {
    int startTileX = static_cast<int>(cameraX_ / GameConst::TILE_SIZE) - 1;
    int endTileX = static_cast<int>((cameraX_ + GameConst::VIEWPORT_WIDTH) / GameConst::TILE_SIZE) + 1;
    int startTileY = 0;
    int endTileY = level_.getHeight();
    
    startTileX = std::max(0, startTileX);
    endTileX = std::min(level_.getWidth(), endTileX);
    
    for (int y = startTileY; y < endTileY; ++y) {
        for (int x = startTileX; x < endTileX; ++x) {
            TileType tile = level_.getTile(x, y);
            if (tile == TileType::EMPTY) continue;
            
            QColor color = getTileColor(tile);
            vec2 pos(x * GameConst::TILE_SIZE, y * GameConst::TILE_SIZE);
            vec2 size(GameConst::TILE_SIZE, GameConst::TILE_SIZE);
            
            renderQuad(pos, size, color.redF(), color.greenF(), color.blueF(), 1.0f);
            
            // Border for depth
            renderQuad(pos, vec2(size.x, 2), 0, 0, 0, 0.2f);
            renderQuad(pos, vec2(2, size.y), 0, 0, 0, 0.2f);
        }
    }
}

void MarioGameWidget::renderPlayer() {
    QColor color = player_.isBig ? QColor(255, 0, 0) : QColor(200, 0, 0); // Red
    renderQuad(player_.position, player_.size, color.redF(), color.greenF(), color.blueF(), 1.0f);
    
    // Eyes (white)
    float eyeSize = 6;
    float eyeY = player_.position.y + 8;
    if (player_.facingRight) {
        renderCircle(vec2(player_.position.x + player_.size.x - 12, eyeY), eyeSize / 2, 1, 1, 1, 1);
    } else {
        renderCircle(vec2(player_.position.x + 8, eyeY), eyeSize / 2, 1, 1, 1, 1);
    }
}

void MarioGameWidget::renderEnemies() {
    for (const auto& enemy : enemies_) {
        if (!enemy.alive) continue;
        
        // Goomba - brown rounded rectangle
        QColor color(139, 90, 43); // Brown
        renderQuad(enemy.position, enemy.size, color.redF(), color.greenF(), color.blueF(), 1.0f);
        
        // Eyes
        renderCircle(vec2(enemy.position.x + 8, enemy.position.y + 10), 3, 1, 1, 1, 1);
        renderCircle(vec2(enemy.position.x + enemy.size.x - 8, enemy.position.y + 10), 3, 1, 1, 1, 1);
    }
}

void MarioGameWidget::renderCoins() {
    for (auto& coin : coins_) {
        if (!coin.alive) continue;
        
        // Animate coin rotation
        coin.animTimer += 0.05f;
        float scale = std::abs(std::sin(coin.animTimer));
        
        vec2 pos = coin.position;
        float width = coin.size.x * scale;
        float offset = (coin.size.x - width) / 2;
        
        // Gold coin
        renderQuad(vec2(pos.x + offset, pos.y), vec2(width, coin.size.y), 1.0f, 0.84f, 0.0f, 1.0f);
    }
}

void MarioGameWidget::renderParticles() {
    for (const auto& p : particles_) {
        if (p.life <= 0.0f) continue;
        
        renderCircle(p.position, p.size, p.r, p.g, p.b, p.a * p.life);
    }
}

void MarioGameWidget::renderHUD() {
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    
    // Draw shadow for readable text
    painter.setPen(Qt::black);
    painter.drawText(11, 26, QString("MARIO"));
    painter.drawText(11, 46, QString("%1").arg(player_.score, 6, 10, QChar('0')));
    
    painter.drawText(211, 26, QString("COINS"));
    painter.drawText(211, 46, QString("x%1").arg(player_.coins));
    
    painter.drawText(411, 26, QString("WORLD"));
    painter.drawText(411, 46, QString("1-1"));
    
    painter.drawText(611, 26, QString("TIME"));
    painter.drawText(611, 46, QString("%1").arg(static_cast<int>(300 - gameTime_)));
    
    // Draw white text on top
    painter.setPen(Qt::white);
    painter.drawText(10, 25, QString("MARIO"));
    painter.drawText(10, 45, QString("%1").arg(player_.score, 6, 10, QChar('0')));
    
    painter.drawText(210, 25, QString("COINS"));
    painter.drawText(210, 45, QString("x%1").arg(player_.coins));
    
    painter.drawText(410, 25, QString("WORLD"));
    painter.drawText(410, 45, QString("1-1"));
    
    painter.drawText(610, 25, QString("TIME"));
    painter.drawText(610, 45, QString("%1").arg(static_cast<int>(300 - gameTime_)));
    
    // Lives indicator
    painter.drawText(10, GameConst::VIEWPORT_HEIGHT - 10, QString("Lives: %1").arg(player_.lives));
    
    // FPS
    painter.drawText(GameConst::VIEWPORT_WIDTH - 100, GameConst::VIEWPORT_HEIGHT - 10, 
                    QString("FPS: %1").arg(static_cast<int>(fps_)));
    
    // Game state messages
    if (state_ == GameState::PAUSED) {
        painter.setFont(QFont("Arial", 32, QFont::Bold));
        painter.fillRect(rect(), QColor(0, 0, 0, 150));
        painter.setPen(Qt::yellow);
        painter.drawText(rect(), Qt::AlignCenter, "PAUSED");
    } else if (state_ == GameState::GAME_OVER) {
        painter.setFont(QFont("Arial", 32, QFont::Bold));
        painter.fillRect(rect(), QColor(0, 0, 0, 200));
        painter.setPen(Qt::red);
        painter.drawText(rect(), Qt::AlignCenter, "GAME OVER\n\nPress R to Restart");
    } else if (state_ == GameState::LEVEL_COMPLETE) {
        painter.setFont(QFont("Arial", 32, QFont::Bold));
        painter.fillRect(rect(), QColor(0, 100, 0, 180));
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, QString("LEVEL COMPLETE!\nScore: %1").arg(player_.score));
    }
}

QColor MarioGameWidget::getTileColor(TileType type) const {
    switch (type) {
        case TileType::GROUND: return QColor(139, 90, 43);   // Brown
        case TileType::BRICK: return QColor(205, 92, 92);    // Brick red
        case TileType::QUESTION: return QColor(255, 215, 0); // Gold
        case TileType::PIPE_TOP_LEFT:
        case TileType::PIPE_TOP_RIGHT:
        case TileType::PIPE_BODY_LEFT:
        case TileType::PIPE_BODY_RIGHT: return QColor(34, 139, 34); // Green
        default: return Qt::gray;
    }
}

void MarioGameWidget::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    
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
        case Qt::Key_Up:
        case Qt::Key_W:
            jumpPressed_ = true;
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            downPressed_ = true;
            break;
        case Qt::Key_P:
            if (state_ == GameState::PLAYING) {
                state_ = GameState::PAUSED;
            } else if (state_ == GameState::PAUSED) {
                state_ = GameState::PLAYING;
            }
            break;
        case Qt::Key_R:
            reset();
            break;
    }
}

void MarioGameWidget::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    
    switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_A:
            leftPressed_ = false;
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            rightPressed_ = false;
            break;
        case Qt::Key_Space:
        case Qt::Key_Up:
        case Qt::Key_W:
            jumpPressed_ = false;
            player_.canJump = true;
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            downPressed_ = false;
            break;
    }
}

