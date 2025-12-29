#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QMatrix4x4>
#include <memory>
#include <vector>
#include "game_types.h"
#include "level_data.h"
#include "physics_engine.h"

class MarioGameWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT
    
public:
    explicit MarioGameWidget(QWidget *parent = nullptr);
    ~MarioGameWidget() override;
    
protected:
    // OpenGL lifecycle
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    
    // Input events
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    
private slots:
    void gameLoop();
    
private:
    // Game logic
    void reset();
    void updatePlayer(float deltaTime);
    void updateEnemies(float deltaTime);
    void updateParticles(float deltaTime);
    void updateCamera();
    void checkCollisions();
    void spawnParticles(vec2 pos, float r, float g, float b);
    
    // OpenGL rendering
    void initShaders();
    void initBuffers();
    void renderTiles();
    void renderPlayer();
    void renderEnemies();
    void renderCoins();
    void renderParticles();
    void renderHUD();
    void renderQuad(vec2 pos, vec2 size, float r, float g, float b, float a);
    void renderCircle(vec2 pos, float radius, float r, float g, float b, float a);
    
    QColor getTileColor(TileType type) const;
    
    // Game state
    GameState state_;
    Player player_;
    std::vector<Enemy> enemies_;
    std::vector<Entity> coins_;
    std::vector<Particle> particles_;
    LevelData level_;
    
    // Camera
    float cameraX_;
    float cameraY_;
    
    // Input state
    bool leftPressed_;
    bool rightPressed_;
    bool jumpPressed_;
    bool downPressed_;
    
    // OpenGL objects
    std::unique_ptr<QOpenGLShaderProgram> shaderProgram_;
    std::unique_ptr<QOpenGLBuffer> vbo_;
    std::unique_ptr<QOpenGLVertexArrayObject> vao_;
    QMatrix4x4 projection_;
    QMatrix4x4 view_;
    
    // Timing
    QTimer *timer_;
    qint64 lastFrameTime_;
    float deltaTime_;
    int frameCount_;
    float fps_;
    float gameTime_;
};
