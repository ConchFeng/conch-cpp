#pragma once
#include "game_types.h"
#include <vector>

class LevelData {
public:
    LevelData();
    
    TileType getTile(int x, int y) const;
    void setTile(int x, int y, TileType type);
    
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    
    vec2 getPlayerStart() const { return playerStart_; }
    const std::vector<vec2>& getEnemySpawns() const { return enemySpawns_; }
    const std::vector<vec2>& getCoinPositions() const { return coinPositions_; }
    
    void loadLevel1();
    
private:
    int width_;
    int height_;
    std::vector<TileType> tiles_;
    vec2 playerStart_;
    std::vector<vec2> enemySpawns_;
    std::vector<vec2> coinPositions_;
};
