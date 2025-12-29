#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QMatrix4x4>
#include <vector>
#include <memory>
#include "game_types.h"

class GLGameWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
    
public:
    explicit GLGameWidget(QWidget *parent = nullptr);
    ~GLGameWidget() override;
    
protected:
    // OpenGL lifecycle
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    
    // Input events
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    
private slots:
    void gameLoop();
    
private:
    // Game logic (same as before)
    void reset();
    void initLevel(int level);
    void launchBall();
    void updateBall();
    void updateParticles(float deltaTime);
    void checkCollisions();
    void checkBrickCollision();
    void checkPaddleCollision();
    void nextLevel();
    void spawnParticles(float x, float y, float r, float g, float b);
    
    // OpenGL rendering
    void initShaders();
    void initBuffers();
    void renderQuad(float x, float y, float w, float h, float r, float g, float b, float a);
    void renderCircle(float x, float y, float radius, float r, float g, float b, float a);
    void renderBricks();
    void renderPaddle();
    void renderBall();
    void renderParticles();
    void renderHUD();
    
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
    std::vector<Particle> particles_;
    int score_;
    int lives_;
    int level_;
    
    // Input state
    bool leftPressed_;
    bool rightPressed_;
    
    // OpenGL objects
    std::unique_ptr<QOpenGLShaderProgram> shaderProgram_;
    std::unique_ptr<QOpenGLBuffer> vbo_;
    std::unique_ptr<QOpenGLVertexArrayObject> vao_;
    QMatrix4x4 projection_;
    
    // Timing
    QTimer *timer_;
    qint64 lastFrameTime_;
    float deltaTime_;
    int frameCount_;
    float fps_;
};
