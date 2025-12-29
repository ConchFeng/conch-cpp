#include "level_data.h"
#include <algorithm>


LevelData::LevelData() : width_(200), height_(20) {
    tiles_.resize(width_ * height_, TileType::EMPTY);
}

TileType LevelData::getTile(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return TileType::EMPTY;
    }
    return tiles_[y * width_ + x];
}

void LevelData::setTile(int x, int y, TileType type) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        tiles_[y * width_ + x] = type;
    }
}

void LevelData::loadLevel1() {
    // Clear everything
    std::fill(tiles_.begin(), tiles_.end(), TileType::EMPTY);
    enemySpawns_.clear();
    coinPositions_.clear();
    
    // Player start position
    playerStart_ = vec2(100, 400);
    
    // Ground level (y = 17, bottom 3 rows are ground)
    for (int x = 0; x < width_; ++x) {
        for (int y = 17; y < height_; ++y) {
            setTile(x, y, TileType::GROUND);
        }
    }
    
    // Platform 1 - floating platform
    for (int x = 20; x < 28; ++x) {
        setTile(x, 14, TileType::GROUND);
    }
    
    // Platform 2 - higher platform
    for (int x = 35; x < 43; ++x) {
        setTile(x, 11, TileType::GROUND);
    }
    
    // Platform 3 - stairs
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j <= i; ++j) {
            setTile(50 + i, 16 - j, TileType::BRICK);
        }
    }
    
    // Question blocks with coins
    setTile(15, 13, TileType::QUESTION);
    setTile(25, 10, TileType::QUESTION);
    setTile(40, 8, TileType::QUESTION);
    
    // Brick blocks
    for (int x = 60; x < 65; ++x) {
        setTile(x, 13, TileType::BRICK);
    }
    
    // Pipe 1
    setTile(70, 15, TileType::PIPE_TOP_LEFT);
    setTile(71, 15, TileType::PIPE_TOP_RIGHT);
    setTile(70, 16, TileType::PIPE_BODY_LEFT);
    setTile(71, 16, TileType::PIPE_BODY_RIGHT);
    
    // Pipe 2 (taller)
    for (int y = 14; y < 17; ++y) {
        if (y == 14) {
            setTile(85, y, TileType::PIPE_TOP_LEFT);
            setTile(86, y, TileType::PIPE_TOP_RIGHT);
        } else {
            setTile(85, y, TileType::PIPE_BODY_LEFT);
            setTile(86, y, TileType::PIPE_BODY_RIGHT);
        }
    }
    
    // Platform near end
    for (int x = 100; x < 110; ++x) {
        setTile(x, 14, TileType::GROUND);
    }
    
    // Final platform with flag
    for (int x = 190; x < 200; ++x) {
        for (int y = 15; y < 17; ++y) {
            setTile(x, y, TileType::BRICK);
        }
    }
    
    // Enemy spawns (Goombas)
    enemySpawns_.push_back(vec2(30 * GameConst::TILE_SIZE, 15 * GameConst::TILE_SIZE));
    enemySpawns_.push_back(vec2(55 * GameConst::TILE_SIZE, 15 * GameConst::TILE_SIZE));
    enemySpawns_.push_back(vec2(90 * GameConst::TILE_SIZE, 15 * GameConst::TILE_SIZE));
    enemySpawns_.push_back(vec2(120 * GameConst::TILE_SIZE, 15 * GameConst::TILE_SIZE));
    
    // Coin positions (floating coins)
    for (int x = 10; x < 15; ++x) {
        coinPositions_.push_back(vec2(x * GameConst::TILE_SIZE + 16, 12 * GameConst::TILE_SIZE + 16));
    }
    
    for (int x = 75; x < 80; ++x) {
        coinPositions_.push_back(vec2(x * GameConst::TILE_SIZE + 16, 10 * GameConst::TILE_SIZE + 16));
    }
    
    // Coins on platforms
    coinPositions_.push_back(vec2(24 * GameConst::TILE_SIZE, 13 * GameConst::TILE_SIZE));
    coinPositions_.push_back(vec2(39 * GameConst::TILE_SIZE, 10 * GameConst::TILE_SIZE));
    coinPositions_.push_back(vec2(105 * GameConst::TILE_SIZE, 13 * GameConst::TILE_SIZE));
}
