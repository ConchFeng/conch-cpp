#include "gl_game_widget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDateTime>
#include <QSurfaceFormat>
#include <cmath>
#include <algorithm>

GLGameWidget::GLGameWidget(QWidget *parent)
    : QOpenGLWidget(parent), state_(GameState::READY), score_(0), lives_(3), level_(1),
      leftPressed_(false), rightPressed_(false), deltaTime_(0.016f), 
      frameCount_(0), fps_(60.0f) {
    
    // Request OpenGL 3.3 Core Profile
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4); // 4x MSAA
    setFormat(format);
    
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &GLGameWidget::gameLoop);
    
    lastFrameTime_ = QDateTime::currentMSecsSinceEpoch();
    
    reset();
}

GLGameWidget::~GLGameWidget() {
    makeCurrent();
    vbo_.reset();
    vao_.reset();
    shaderProgram_.reset();
    doneCurrent();
}

void GLGameWidget::initializeGL() {
    initializeOpenGLFunctions();
    
    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    
    initShaders();
    initBuffers();
    
    timer_->start(16); // ~60 FPS
}

void GLGameWidget::initShaders() {
    shaderProgram_ = std::make_unique<QOpenGLShaderProgram>();
    
    // Vertex shader
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec4 aColor;
        
        uniform mat4 projection;
        
        out vec4 vertexColor;
        
        void main() {
            gl_Position = projection * vec4(aPos, 0.0, 1.0);
            vertexColor = aColor;
        }
    )";
    
    // Fragment shader
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec4 vertexColor;
        out vec4 FragColor;
        
        void main() {
            FragColor = vertexColor;
        }
    )";
    
    if (!shaderProgram_->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qWarning() << "Vertex shader compilation failed:" << shaderProgram_->log();
    }
    
    if (!shaderProgram_->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
        qWarning() << "Fragment shader compilation failed:" << shaderProgram_->log();
    }
    
    if (!shaderProgram_->link()) {
        qWarning() << "Shader program linking failed:" << shaderProgram_->log();
    }
}

void GLGameWidget::initBuffers() {
    vao_ = std::make_unique<QOpenGLVertexArrayObject>();
    vao_->create();
    vao_->bind();
    
    vbo_ = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
    vbo_->create();
    vbo_->bind();
    vbo_->setUsagePattern(QOpenGLBuffer::DynamicDraw);
    
    // Allocate buffer (will be filled dynamically)
    vbo_->allocate(10000 * sizeof(float));
    
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    
    // Color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    
    vbo_->release();
    vao_->release();
}

void GLGameWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    
    // Orthographic projection
    projection_.setToIdentity();
    projection_.ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 
                     static_cast<float>(WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);
}

void GLGameWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    shaderProgram_->bind();
    shaderProgram_->setUniformValue("projection", projection_);
    vao_->bind();
    
    // Render game objects
    renderBricks();
    renderPaddle();
    renderBall();
    renderParticles();
    
    vao_->release();
    shaderProgram_->release();
    
    // Render HUD with QPainter
    renderHUD();
    
    // Calculate FPS
    frameCount_++;
    if (frameCount_ >= 30) {
        fps_ = 30.0f / (deltaTime_ * frameCount_);
        frameCount_ = 0;
    }
}

void GLGameWidget::renderQuad(float x, float y, float w, float h, 
                             float r, float g, float b, float a) {
    float vertices[] = {
        // Pos        // Color
        x,     y,     r, g, b, a,
        x + w, y,     r, g, b, a,
        x + w, y + h, r, g, b, a,
        
        x,     y,     r, g, b, a,
        x + w, y + h, r, g, b, a,
        x,     y + h, r, g, b, a,
    };
    
    vbo_->bind();
    vbo_->write(0, vertices, sizeof(vertices));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GLGameWidget::renderCircle(float x, float y, float radius, 
                                float r, float g, float b, float a) {
    const int segments = 32;
    std::vector<float> vertices;
    vertices.reserve(segments * 3 * 6); // 3 vertices per triangle, 6 floats per vertex
    
    for (int i = 0; i < segments; ++i) {
        float angle1 = (i / static_cast<float>(segments)) * 2.0f * M_PI;
        float angle2 = ((i + 1) / static_cast<float>(segments)) * 2.0f * M_PI;
        
        // Center
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(r);
        vertices.push_back(g);
        vertices.push_back(b);
        vertices.push_back(a);
        
        // Point 1
        vertices.push_back(x + radius * std::cos(angle1));
        vertices.push_back(y + radius * std::sin(angle1));
        vertices.push_back(r);
        vertices.push_back(g);
        vertices.push_back(b);
        vertices.push_back(a);
        
        // Point 2
        vertices.push_back(x + radius * std::cos(angle2));
        vertices.push_back(y + radius * std::sin(angle2));
        vertices.push_back(r);
        vertices.push_back(g);
        vertices.push_back(b);
        vertices.push_back(a);
    }
    
    vbo_->bind();
    vbo_->write(0, vertices.data(), vertices.size() * sizeof(float));
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);
}

void GLGameWidget::renderBricks() {
    for (const auto& brick : bricks_) {
        if (brick.destroyed && brick.alpha <= 0.0f) continue;
        
        float x = BRICK_OFFSET_LEFT + brick.col * (BRICK_WIDTH + BRICK_PADDING);
        float y = BRICK_OFFSET_TOP + brick.row * (BRICK_HEIGHT + BRICK_PADDING);
        
        QColor color = getBrickColor(brick.hits);
        float alpha = brick.destroyed ? brick.alpha : 1.0f;
        
        renderQuad(x, y, BRICK_WIDTH, BRICK_HEIGHT, 
                  color.redF(), color.greenF(), color.blueF(), alpha);
        
        // Border
        float borderSize = 2.0f;
        renderQuad(x, y, BRICK_WIDTH, borderSize, 1.0f, 1.0f, 1.0f, 0.3f * alpha);
        renderQuad(x, y + BRICK_HEIGHT - borderSize, BRICK_WIDTH, borderSize, 1.0f, 1.0f, 1.0f, 0.3f * alpha);
        renderQuad(x, y, borderSize, BRICK_HEIGHT, 1.0f, 1.0f, 1.0f, 0.3f * alpha);
        renderQuad(x + BRICK_WIDTH - borderSize, y, borderSize, BRICK_HEIGHT, 1.0f, 1.0f, 1.0f, 0.3f * alpha);
    }
}

void GLGameWidget::renderPaddle() {
    renderQuad(paddle_.x - paddle_.width / 2, paddle_.y - paddle_.height / 2,
              paddle_.width, paddle_.height, 0.39f, 0.58f, 0.93f, 1.0f);
    
    // Glow effect
    renderQuad(paddle_.x - paddle_.width / 2 - 2, paddle_.y - paddle_.height / 2 - 2,
              paddle_.width + 4, paddle_.height + 4, 0.39f, 0.58f, 0.93f, 0.3f);
}

void GLGameWidget::renderBall() {
    // Main ball
    renderCircle(ball_.pos.x, ball_.pos.y, ball_.radius, 1.0f, 1.0f, 1.0f, 1.0f);
    
    // Glow effect
    if (ball_.launched) {
        renderCircle(ball_.pos.x, ball_.pos.y, ball_.radius * 1.5f, 1.0f, 1.0f, 1.0f, 0.3f);
        renderCircle(ball_.pos.x, ball_.pos.y, ball_.radius * 2.0f, 1.0f, 1.0f, 1.0f, 0.1f);
    }
}

void GLGameWidget::renderParticles() {
    for (const auto& p : particles_) {
        if (p.life <= 0.0f) continue;
        
        renderCircle(p.pos.x, p.pos.y, p.size, p.r, p.g, p.b, p.a * p.life);
    }
}

void GLGameWidget::renderHUD() {
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14));
    
    painter.drawText(10, 30, QString("Score: %1").arg(score_));
    painter.drawText(200, 30, QString("Lives: %1").arg(lives_));
    painter.drawText(350, 30, QString("Level: %1").arg(level_));
    painter.drawText(550, 30, QString("FPS: %1").arg(static_cast<int>(fps_)));
    
    if (state_ == GameState::READY) {
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "Press SPACE to launch ball");
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

// Game logic methods (same as original, copy from game_widget.cpp)
void GLGameWidget::reset() {
    paddle_.x = WINDOW_WIDTH / 2.0f;
    paddle_.y = WINDOW_HEIGHT - 40;
    paddle_.width = 100;
    paddle_.height = 15;
    
    ball_.pos.x = paddle_.x;
    ball_.pos.y = paddle_.y - paddle_.height / 2 - ball_.radius - 2;
    ball_.vel.dx = 0;
    ball_.vel.dy = 0;
    ball_.launched = false;
    
    score_ = 0;
    lives_ = 3;
    level_ = 1;
    state_ = GameState::READY;
    particles_.clear();
    
    initLevel(level_);
}

void GLGameWidget::initLevel(int level) {
    bricks_.clear();
    
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            if (level > 1 && (row + col) % (level + 2) == 0) {
                continue;
            }
            
            int hits = 1;
            if (row < 2) {
                hits = 3;
            } else if (row < 4) {
                hits = 2;
            }
            
            if (level > 1) {
                hits = std::min(3, hits + (level - 1) / 2);
            }
            
            bricks_.emplace_back(row, col, hits);
        }
    }
}

void GLGameWidget::launchBall() {
    if (!ball_.launched) {
        ball_.launched = true;
        float angle = -1.3f;
        ball_.vel.dx = BALL_SPEED * std::sin(angle);
        ball_.vel.dy = BALL_SPEED * std::cos(angle);
    }
}

void GLGameWidget::updateBall() {
    if (!ball_.launched) {
        ball_.pos.x = paddle_.x;
        ball_.pos.y = paddle_.y - paddle_.height / 2 - ball_.radius - 2;
        return;
    }
    
    ball_.pos.x += ball_.vel.dx;
    ball_.pos.y += ball_.vel.dy;
    
    if (ball_.pos.x - ball_.radius <= 0 || ball_.pos.x + ball_.radius >= WINDOW_WIDTH) {
        ball_.vel.dx = -ball_.vel.dx;
        ball_.pos.x = std::clamp(ball_.pos.x, ball_.radius, WINDOW_WIDTH - ball_.radius);
    }
    
    if (ball_.pos.y - ball_.radius <= 0) {
        ball_.vel.dy = -ball_.vel.dy;
        ball_.pos.y = ball_.radius;
    }
    
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

void GLGameWidget::checkPaddleCollision() {
    if (!ball_.launched) return;
    
    if (ball_.pos.y + ball_.radius >= paddle_.y - paddle_.height / 2 &&
        ball_.pos.y + ball_.radius <= paddle_.y + paddle_.height / 2 + 5 &&
        ball_.vel.dy > 0) {
        
        if (ball_.pos.x >= paddle_.x - paddle_.width / 2 &&
            ball_.pos.x <= paddle_.x + paddle_.width / 2) {
            
            ball_.vel.dy = -std::abs(ball_.vel.dy);
            
            float hitPos = (ball_.pos.x - paddle_.x) / (paddle_.width / 2);
            ball_.vel.dx = hitPos * BALL_SPEED * 0.75f;
            
            float speed = std::sqrt(ball_.vel.dx * ball_.vel.dx + ball_.vel.dy * ball_.vel.dy);
            ball_.vel.dx = (ball_.vel.dx / speed) * BALL_SPEED;
            ball_.vel.dy = (ball_.vel.dy / speed) * BALL_SPEED;
            
            ball_.pos.y = paddle_.y - paddle_.height / 2 - ball_.radius;
        }
    }
}

void GLGameWidget::checkBrickCollision() {
    if (!ball_.launched) return;
    
    for (auto& brick : bricks_) {
        if (brick.destroyed) continue;
        
        float brickX = BRICK_OFFSET_LEFT + brick.col * (BRICK_WIDTH + BRICK_PADDING);
        float brickY = BRICK_OFFSET_TOP + brick.row * (BRICK_HEIGHT + BRICK_PADDING);
        
        if (ball_.pos.x + ball_.radius >= brickX &&
            ball_.pos.x - ball_.radius <= brickX + BRICK_WIDTH &&
            ball_.pos.y + ball_.radius >= brickY &&
            ball_.pos.y - ball_.radius <= brickY + BRICK_HEIGHT) {
            
            QColor color = getBrickColor(brick.hits);
            spawnParticles(brickX + BRICK_WIDTH / 2, brickY + BRICK_HEIGHT / 2,
                          color.redF(), color.greenF(), color.blueF());
            
            brick.hits--;
            if (brick.hits <= 0) {
                brick.destroyed = true;
            }
            
            score_ += getBrickScore(brick.hits + 1);
            
            float dx = ball_.pos.x - (brickX + BRICK_WIDTH / 2);
            float dy = ball_.pos.y - (brickY + BRICK_HEIGHT / 2);
            
            if (std::abs(dx / BRICK_WIDTH) > std::abs(dy / BRICK_HEIGHT)) {
                ball_.vel.dx = -ball_.vel.dx;
            } else {
                ball_.vel.dy = -ball_.vel.dy;
            }
            
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
            
            break;
        }
    }
}

void GLGameWidget::checkCollisions() {
    checkPaddleCollision();
    checkBrickCollision();
}

void GLGameWidget::spawnParticles(float x, float y, float r, float g, float b) {
    for (int i = 0; i < 20; ++i) {
        Particle p;
        p.pos.x = x;
        p.pos.y = y;
        float angle = (i / 20.0f) * 2.0f * M_PI;
        float speed = 2.0f + (rand() % 100) / 100.0f * 2.0f;
        p.vel.dx = std::cos(angle) * speed;
        p.vel.dy = std::sin(angle) * speed;
        p.r = r;
        p.g = g;
        p.b = b;
        p.a = 1.0f;
        p.life = 1.0f;
        p.size = 2.0f + (rand() % 100) / 100.0f * 2.0f;
        particles_.push_back(p);
    }
}

void GLGameWidget::updateParticles(float deltaTime) {
    for (auto& p : particles_) {
        if (p.life <= 0.0f) continue;
        
        p.pos.x += p.vel.dx;
        p.pos.y += p.vel.dy;
        p.vel.dy += 0.2f; // Gravity
        p.life -= deltaTime * 2.0f;
    }
    
    // Remove dead particles
    particles_.erase(
        std::remove_if(particles_.begin(), particles_.end(),
                      [](const Particle& p) { return p.life <= 0.0f; }),
        particles_.end()
    );
}

void GLGameWidget::nextLevel() {
    level_++;
    ball_.launched = false;
    ball_.pos.x = paddle_.x;
    ball_.pos.y = paddle_.y - paddle_.height / 2 - ball_.radius - 2;
    ball_.vel.dx = 0;
    ball_.vel.dy = 0;
    particles_.clear();
    initLevel(level_);
    state_ = GameState::READY;
}

void GLGameWidget::gameLoop() {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    deltaTime_ = (currentTime - lastFrameTime_) / 1000.0f;
    lastFrameTime_ = currentTime;
    
    if (state_ == GameState::PLAYING) {
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
    
    updateParticles(deltaTime_);
    
    // Fade out destroyed bricks
    for (auto& brick : bricks_) {
        if (brick.destroyed && brick.alpha > 0.0f) {
            brick.alpha -= deltaTime_ * 3.0f;
        }
    }
    
    update();
}

QColor GLGameWidget::getBrickColor(int hits) const {
    switch (hits) {
        case 3: return QColor(220, 20, 60);
        case 2: return QColor(255, 165, 0);
        case 1: return QColor(50, 205, 50);
        default: return Qt::gray;
    }
}

int GLGameWidget::getBrickScore(int hits) const {
    return hits * 10;
}

void GLGameWidget::keyPressEvent(QKeyEvent *event) {
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

void GLGameWidget::keyReleaseEvent(QKeyEvent *event) {
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
    }
}

void GLGameWidget::mouseMoveEvent(QMouseEvent *event) {
    if (state_ == GameState::PLAYING || state_ == GameState::READY) {
        paddle_.x = std::clamp(static_cast<float>(event->pos().x()), 
                              paddle_.width / 2, 
                              WINDOW_WIDTH - paddle_.width / 2);
    }
}
