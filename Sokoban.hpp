//  <Copyright> <Ashton Roxas>
#pragma once
#ifndef _HOME_ASHTONZKIE_DOCUMENTS_PS3B_SOKOBAN_HPP_
#define _HOME_ASHTONZKIE_DOCUMENTS_PS3B_SOKOBAN_HPP_

#include <iostream>
#include <stack>
#include <SFML/Graphics.hpp>

namespace SB {
    enum class Direction {
      Up,      // W
      Down,    // S
      Left,    // A
      Right    // D
      };

class Sokoban : public sf::Drawable {
 public:
    static const size_t TILE = 64;
    explicit Sokoban(const std::string& filename);
    size_t width() const { return levelWidth; }
    size_t height() const { return levelHeight; }
    size_t pixelWidth() const { return levelWidth * TILE; }
    size_t pixelHeight() const { return levelHeight * TILE; }
    Sokoban();  /* Default Constructor*/
    sf::Vector2i playerLoc() const {return playerPosition; }
    bool isWon() const;
    void levelReset(const std::string& filename);
    friend std::istream& operator>>(std::istream& is, Sokoban& sokoban);
    void movePlayer(Direction direction);
    bool moveBox(Direction direction, int boxX, int boxY);
    void setWindowSize(const sf::Vector2i& size);
    void loadTextures();
    bool loadLevelFromFile(const std::string& filename);
    void setSize(size_t width, size_t height);
    void addTarget(size_t x, size_t y);

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
    size_t levelHeight;
    size_t levelWidth;
    const float tileSize = 64.0f;
    sf::Texture blockTexture;
    sf::Texture crateTexture;
    sf::Texture enviTexture;
    sf::Texture groundTexture;
    sf::Texture targetTexture;
    sf::Texture frontTexture;
    sf::Texture backTexture;
    sf::Texture leftTexture;
    sf::Texture rightTexture;

    sf::Vector2i windowSize;
    sf::Vector2i playerPosition;

    std::vector<std::vector<char>> levelData;
    std::vector<sf::Vector2i> boxPositions;

    Direction playerDirection;
};

#endif  // _HOME_ASHTONZKIE_DOCUMENTS_PS3B_SOKOBAN_HPP"
}  // namespace SB
