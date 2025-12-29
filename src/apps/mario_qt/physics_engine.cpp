#include "physics_engine.h"
#include <algorithm>
#include <cmath>

void PhysicsEngine::applyGravity(Entity& entity, float deltaTime) {
    if (!entity.onGround) {
        entity.velocity.y += GameConst::GRAVITY * deltaTime * 60.0f;
        entity.velocity.y = std::min(entity.velocity.y, GameConst::MAX_FALL_SPEED);
    }
}

void PhysicsEngine::integrate(Entity& entity, float deltaTime) {
    entity.position += entity.velocity * deltaTime * 60.0f;
}

bool PhysicsEngine::isSolidTile(TileType type) {
    return type == TileType::GROUND || type == TileType::BRICK || 
           type == TileType::QUESTION ||
           type == TileType::PIPE_TOP_LEFT || type == TileType::PIPE_TOP_RIGHT ||
           type == TileType::PIPE_BODY_LEFT || type == TileType::PIPE_BODY_RIGHT;
}

AABB PhysicsEngine::getTileAABB(int tileX, int tileY) {
    return AABB(vec2(tileX * GameConst::TILE_SIZE, tileY * GameConst::TILE_SIZE), 
                vec2(GameConst::TILE_SIZE, GameConst::TILE_SIZE));
}

bool PhysicsEngine::checkTileCollision(Entity& entity, const LevelData& level, float deltaTime) {
    entity.onGround = false;
    
    AABB entityBox = entity.getAABB();
    
    // Get tile range to check
    int minTileX = static_cast<int>(std::floor(entityBox.min.x / GameConst::TILE_SIZE));
    int maxTileX = static_cast<int>(std::ceil(entityBox.max.x / GameConst::TILE_SIZE));
    int minTileY = static_cast<int>(std::floor(entityBox.min.y / GameConst::TILE_SIZE));
    int maxTileY = static_cast<int>(std::ceil(entityBox.max.y / GameConst::TILE_SIZE));
    
    // Clamp to level bounds
    minTileX = std::max(0, minTileX);
    maxTileX = std::min(level.getWidth(), maxTileX);
    minTileY = std::max(0, minTileY);
    maxTileY = std::min(level.getHeight(), maxTileY);
    
    vec2 correction(0, 0);
    bool collided = false;
    
    // Check horizontal collisions first
    for (int ty = minTileY; ty < maxTileY; ++ty) {
        for (int tx = minTileX; tx < maxTileX; ++tx) {
            TileType tile = level.getTile(tx, ty);
            if (!isSolidTile(tile)) continue;
            
            AABB tileBox = getTileAABB(tx, ty);
            if (entityBox.intersects(tileBox)) {
                // Calculate overlap
                float overlapLeft = entityBox.max.x - tileBox.min.x;
                float overlapRight = tileBox.max.x - entityBox.min.x;
                float overlapTop = entityBox.max.y - tileBox.min.y;
                float overlapBottom = tileBox.max.y - entityBox.min.y;
                
                // Find minimum overlap
                float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
                
                if (minOverlap == overlapLeft && entity.velocity.x > 0) {
                    correction.x = -overlapLeft;
                    entity.velocity.x = 0;
                    collided = true;
                } else if (minOverlap == overlapRight && entity.velocity.x < 0) {
                    correction.x = overlapRight;
                    entity.velocity.x = 0;
                    collided = true;
                } else if (minOverlap == overlapTop && entity.velocity.y > 0) {
                    correction.y = -overlapTop;
                    entity.velocity.y = 0;
                    entity.onGround = true;
                    collided = true;
                } else if (minOverlap == overlapBottom && entity.velocity.y < 0) {
                    correction.y = overlapBottom;
                    entity.velocity.y = 0;
                    collided = true;
                }
            }
        }
    }
    
    // Apply correction
    entity.position += correction;
    
    return collided;
}

bool PhysicsEngine::checkEntityCollision(const Entity& a, const Entity& b) {
    return a.getAABB().intersects(b.getAABB());
}

bool PhysicsEngine::isOnGround(const Entity& entity, const LevelData& level) {
    // Check one pixel below
    AABB checkBox = entity.getAABB();
    checkBox.min.y += 1;
    checkBox.max.y += 1;
    
    int minTileX = static_cast<int>(std::floor(checkBox.min.x / GameConst::TILE_SIZE));
    int maxTileX = static_cast<int>(std::ceil(checkBox.max.x / GameConst::TILE_SIZE));
    int tileY = static_cast<int>(std::floor(checkBox.max.y / GameConst::TILE_SIZE));
    
    for (int tx = minTileX; tx < maxTileX; ++tx) {
        TileType tile = level.getTile(tx, tileY);
        if (isSolidTile(tile)) {
            return true;
        }
    }
    
    return false;
}
