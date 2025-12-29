#pragma once
#include <cmath>

// Math utilities
struct vec2 {
    float x, y;
    
    vec2() : x(0), y(0) {}
    vec2(float x_, float y_) : x(x_), y(y_) {}
    
    vec2 operator+(const vec2& other) const { return vec2(x + other.x, y + other.y); }
    vec2 operator-(const vec2& other) const { return vec2(x - other.x, y - other.y); }
    vec2 operator*(float scalar) const { return vec2(x * scalar, y * scalar); }
    vec2& operator+=(const vec2& other) { x += other.x; y += other.y; return *this; }
    vec2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    
    float length() const { return std::sqrt(x * x + y * y); }
    vec2 normalized() const { float len = length(); return len > 0 ? vec2(x / len, y / len) : vec2(0, 0); }
};

struct AABB {
    vec2 min, max;
    
    AABB() {}
    AABB(vec2 pos, vec2 size) : min(pos), max(pos + size) {}
    
    bool intersects(const AABB& other) const {
        return !(max.x < other.min.x || min.x > other.max.x ||
                max.y < other.min.y || min.y > other.max.y);
    }
};

// Game constants
namespace GameConst {
    constexpr float GRAVITY = 0.6f;
    constexpr float MAX_FALL_SPEED = 16.0f;
    constexpr float JUMP_VELOCITY = -13.0f;
    constexpr float MOVE_ACCELERATION = 0.8f;
    constexpr float MOVE_DECELERATION = 0.9f;
    constexpr float MAX_MOVE_SPEED = 5.0f;
    constexpr float TILE_SIZE = 32.0f;
    constexpr int VIEWPORT_WIDTH = 800;
    constexpr int VIEWPORT_HEIGHT = 600;
}

// Tile types
enum class TileType {
    EMPTY = 0,
    GROUND,
    BRICK,
    QUESTION,
    PIPE_TOP_LEFT,
    PIPE_TOP_RIGHT,
    PIPE_BODY_LEFT,
    PIPE_BODY_RIGHT,
    COIN
};

// Entity types
enum class EntityType {
    PLAYER,
    GOOMBA,
    KOOPA,
    COIN,
    MUSHROOM
};

// Player state
enum class PlayerState {
    IDLE,
    WALKING,
    JUMPING,
    FALLING,
    CROUCHING,
    DEAD
};

// Base entity
struct Entity {
    vec2 position;
    vec2 velocity;
    vec2 size;
    EntityType type;
    bool onGround;
    bool alive;
    bool facingRight;
    float animTimer;
    int animFrame;
    
    Entity() : position(0, 0), velocity(0, 0), size(0, 0), 
               type(EntityType::PLAYER), onGround(false), alive(true),
               facingRight(true), animTimer(0), animFrame(0) {}
    
    AABB getAABB() const {
        return AABB(position, size);
    }
};

// Player specific
struct Player : public Entity {
    PlayerState state;
    bool isBig;
    bool canJump;
    float jumpHoldTime;
    int lives;
    int coins;
    int score;
    
    Player() : state(PlayerState::IDLE), isBig(false), canJump(true),
               jumpHoldTime(0), lives(3), coins(0), score(0) {
        type = EntityType::PLAYER;
        size = vec2(28, 28);  // Small Mario size
    }
};

// Enemy specific
struct Enemy : public Entity {
    float patrolLeft, patrolRight;
    bool movingRight;
    
    Enemy() : patrolLeft(0), patrolRight(0), movingRight(true) {}
};

// Particle for effects
struct Particle {
    vec2 position;
    vec2 velocity;
    float r, g, b, a;
    float life;
    float size;
    
    Particle() : position(0, 0), velocity(0, 0), 
                 r(1), g(1), b(1), a(1), life(1.0f), size(4.0f) {}
};

// Game state
enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    LEVEL_COMPLETE,
    GAME_OVER
};
