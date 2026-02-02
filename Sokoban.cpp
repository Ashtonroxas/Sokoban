//  <Copyright> <Ashton Roxas>
#include "Sokoban.hpp"
#include <fstream>
#include <algorithm>

namespace SB {
    Sokoban::Sokoban(const std::string& filename) {
        loadLevelFromFile(filename);
        loadTextures();
    }

    Sokoban::Sokoban() { }

    void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // std::cout << playerPosition.x << " " << playerPosition.y << std::endl;
    // Draw the ground tiles
    for (size_t y = 0; y < levelHeight; y++) {
        for (size_t x = 0; x < levelWidth; x++) {
            sf::Sprite backgroundTile;
            sf::Vector2f tilePosition(x * tileSize, y * tileSize);
            backgroundTile.setTexture(groundTexture);
            backgroundTile.setPosition(tilePosition);
            target.draw(backgroundTile, states);
        }
    }
    // Draw the textures on top of the background tiles
    for (size_t y = 0; y < levelHeight; y++) {
        for (size_t x = 0; x < levelWidth; x++) {
            sf::Vector2f tilePosition(x * tileSize, y * tileSize);
            sf::Sprite TILE;

            // Set the appropriate texture based on levelData
            if (levelData[y][x] == '.') {
                TILE.setTexture(groundTexture);
            } else if (levelData[y][x] == '#') {
                TILE.setTexture(blockTexture);
            } else if (levelData[y][x] == 'A') {
                TILE.setTexture(crateTexture);
            } else if (levelData[y][x] == 'a') {
                TILE.setTexture(targetTexture);
            }

            // Set the position of the texture sprite
            TILE.setPosition(tilePosition);
            // Draw the texture sprite
            target.draw(TILE, states);
        }
    }
        // Draw the player sprite
        sf::Vector2f playerPixelPosition(playerPosition.x * tileSize, playerPosition.y * tileSize);
        sf::Sprite playerSprite;

        playerSprite.setTexture(frontTexture);

        switch (playerDirection) {
            case Direction::Up:
                playerSprite.setTexture(backTexture);
                break;
            case Direction::Down:
                playerSprite.setTexture(frontTexture);
                break;
            case Direction::Left:
                playerSprite.setTexture(rightTexture);
                break;
            case Direction::Right:
                playerSprite.setTexture(leftTexture);
                break;
        }
            // Set the position of the player sprite
            playerSprite.setPosition(playerPixelPosition);
            // Draw the player sprite
            target.draw(playerSprite, states);
    }

    bool Sokoban::isWon() const {
    // Check if all target positions are covered by crates
    for (size_t y = 0; y < levelHeight; y++) {
        for (size_t x = 0; x < levelWidth; x++) {
            if (levelData[y][x] == 'a') {
                // If any target position is not covered by a crate, return false
                if (levelData[y][x] != 'A') {
                    return false;
                }
            }
        }
    }
    return true;
}

    void Sokoban::levelReset(const std::string& filename) {
        // Reload level data from the file
        if (!loadLevelFromFile(filename)) {
            std::cerr << "Failed to reset level: Unable to load level file "
            << filename << std::endl;
            return;
        }

        // Reset player position to the starting position
        for (size_t y = 0; y < levelHeight; y++) {
            for (size_t x = 0; x < levelWidth; x++) {
                if (levelData[y][x] == '@') {
                    playerPosition = {static_cast<int>(x), static_cast<int>(y)};
                    return;  // Found player position, exit the loop
                }
            }
        }
    }

    void Sokoban::movePlayer(Direction direction) {
        int dx = 0, dy = 0;
        playerDirection = direction;
        // Determine movement direction
        switch (direction) {
            case Direction::Up:
                dy = -1;
                break;
            case Direction::Down:
                dy = 1;
                break;
            case Direction::Left:
                dx = -1;
                break;
            case Direction::Right:
                dx = 1;
                break;
        }


        // Calculate new player position
        size_t newPlayerX = playerPosition.x + dx;
        size_t newPlayerY = playerPosition.y + dy;

        // Check if the new position is within bounds
        if (newPlayerX >= 0 && newPlayerX < levelWidth &&
            newPlayerY >= 0 && newPlayerY <
            levelHeight) {  // Check if the new position is not a wall
            if (levelData[newPlayerY][newPlayerX] != '#') {
                if (levelData[newPlayerY][newPlayerX] != 'A') {  // Update player position
                    playerPosition.x = newPlayerX;
                    playerPosition.y = newPlayerY;
                }
            }
            if (levelData[newPlayerY][newPlayerX] == 'A') {
                if (moveBox(direction, newPlayerX, newPlayerY)) {
                    playerPosition.x = newPlayerX;
                    playerPosition.y = newPlayerY;
                }
            }
        }
    }

    bool Sokoban::moveBox(Direction direction, int boxX, int boxY) {
    int dx = 0, dy = 0;

    // movement direction
    switch (direction) {
        case Direction::Up:
            dy = -1;
            break;
        case Direction::Down:
            dy = 1;
            break;
        case Direction::Left:
            dx = -1;
            break;
        case Direction::Right:
            dx = 1;
            break;
    }

    // Calculate new box position
    size_t newBoxX = boxX + dx;
    size_t newBoxY = boxY + dy;

    // Check if the new position is within bounds
    if (newBoxX >= 0 && newBoxX < levelWidth &&
        newBoxY >= 0 && newBoxY < levelHeight) {
        // Check if the new position is empty or a target
        if (levelData[newBoxY][newBoxX] == '.' || levelData[newBoxY][newBoxX] == 'a') {
            // Move the box
            levelData[newBoxY][newBoxX] = 'A';  // Place crate in the new position
            levelData[boxY][boxX] = '.';  // Clear the old box position
            return true;
        }
    }

    return false;
}

    void Sokoban::setWindowSize(const sf::Vector2i& size) {
        windowSize = size;
    }

    void Sokoban::setSize(size_t width, size_t height) {
        levelWidth = width;
        levelHeight = height;
        levelData.resize(height, std::vector<char>(width, '.'));
    }

    void Sokoban::addTarget(size_t x, size_t y) {
        if (x >= 0 && x < levelWidth && y >= 0 && y < levelHeight) {
            levelData[y][x] = 'a';  /*Assuming 'a' represents a target*/
        }
    }

    void Sokoban::loadTextures() {
        if (!blockTexture.loadFromFile
            ("sokoban/block_06.png")) {
            throw std::runtime_error
            ("Failed to load texture: sokoban/block.png");
        }

        if (!crateTexture.loadFromFile
            ("sokoban/crate_03.png")) {
            throw std::runtime_error
            ("Failed to load texture: sokoban/crate_03.png");
        }
        if (!enviTexture.loadFromFile
            ("sokoban/environment_03.png")) {
            throw std::runtime_error
            ("Failed to load texture: sokoban/environment_03.png");
        }
        if (!groundTexture.loadFromFile
            ("sokoban/ground_01.png")) {
            throw std::runtime_error
            ("Failed to load texture: sokoban/ground_01.png");
        }
        if (!targetTexture.loadFromFile
            ("sokoban/ground_04.png")) {
            throw std::runtime_error
            ("Failed to load texture: sokoban/ground_04.png");
        }
        if (!frontTexture.loadFromFile
            ("sokoban/player_05.png")) {
            throw std::runtime_error
            ("Failed to load texture: sokoban/player_05.png");
        }
        if (!backTexture.loadFromFile
            ("sokoban/player_08.png")) {
            throw std::runtime_error
            ("Failed to load texture: sokoban/player_08.png");
        }
        if (!leftTexture.loadFromFile
            ("sokoban/player_17.png")) {
            throw std::runtime_error
            ("Failed to load texture: sokoban/player_17.png");
        }
        if (!rightTexture.loadFromFile
            ("sokoban/player_20.png")) {
            throw std::runtime_error
            ("Failed to load texture: sokoban/player_20.png");
        }
    }

    bool Sokoban::loadLevelFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        // Read level dimensions from file
        int width, height;
        if (!(file >> height >> width) || width <= 0 || height <= 0) {
            std::cerr << "Invalid level dimensions in file: " << filename << std::endl;
            return false;
        }

        // Resize levelData vector
        levelWidth = width;
        levelHeight = height;
        levelData.resize(height, std::vector<char>(width));

        // Read level data from file
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (!(file >> levelData[y][x])) {
                    std::cerr << "Error reading level data from file: " << filename << std::endl;
                    return false;
                }
                if (levelData[y][x] == '@') {
                    playerPosition = {x, y};
                }
            }
        }
        file.close();  // Close the file after reading
        return true;
    }

    std::istream& operator>>(std::istream& is, Sokoban& sokoban) {
        int width, height;
        is >> height >> width;
        sokoban.levelWidth = width;
        sokoban.levelHeight = height;
        sokoban.levelData.resize(height, std::vector<char>(width));

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                is >> sokoban.levelData[y][x];
                if (sokoban.levelData[y][x] == '@') {
                    sokoban.playerPosition = {x, y};
                }
            }
        }
        return is;
    }

}   /*namespace SB*/
