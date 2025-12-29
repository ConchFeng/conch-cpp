#pragma once

// Common game types shared between QPainter and OpenGL implementations

enum class GameState { READY, PLAYING, PAUSED, GAME_OVER, LEVEL_COMPLETE };

struct Point {
    float x, y;
};

struct Velocity {
    float dx, dy;
};

struct Brick {
    int row, col;
    int hits;
    bool destroyed;
    float alpha; // For fade-out animation
    
    Brick(int r, int c, int h) : row(r), col(c), hits(h), destroyed(false), alpha(1.0f) {}
};

struct Paddle {
    float x, y;
    float width, height;
    
    Paddle() : x(0), y(0), width(100), height(15) {}
};

struct Ball {
    Point pos;
    Velocity vel;
    float radius;
    bool launched;
    
    Ball() : pos{0, 0}, vel{0, 0}, radius(8), launched(false) {}
};

// OpenGL-specific particle struct
struct Particle {
    Point pos;
    Velocity vel;
    float r, g, b, a;
    float life;
    float size;
    
    Particle() : pos{0, 0}, vel{0, 0}, r(1), g(1), b(1), a(1), life(1.0f), size(4.0f) {}
};
