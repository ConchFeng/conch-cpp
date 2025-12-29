#pragma once
#include "game_types.h"
#include "level_data.h"

class PhysicsEngine {
public:
    // Apply gravity to entity
    static void applyGravity(Entity& entity, float deltaTime);
    
    // Update entity position based on velocity
    static void integrate(Entity& entity, float deltaTime);
    
    // Check and resolve collision with tiles
    static bool checkTileCollision(Entity& entity, const LevelData& level, float deltaTime);
    
    // Check collision between two entities
    static bool checkEntityCollision(const Entity& a, const Entity& b);
    
    // Resolve collision response
    static void resolveCollision(Entity& entity, const AABB& obstacle, vec2& correction);
    
    // Get ground beneath entity
    static bool isOnGround(const Entity& entity, const LevelData& level);
    
private:
    static bool isSolidTile(TileType type);
    static AABB getTileAABB(int tileX, int tileY);
};
