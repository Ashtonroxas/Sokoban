//  <Copyright> <Ashton Roxas>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Sokoban.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <level_file>" << std::endl;
        return 1;
    }

    std::string lvlFile = argv[1];
    SB::Sokoban game(lvlFile);

    // Set window size after loading level data
    sf::Vector2i windowSize(game.pixelWidth(), game.pixelHeight());
    game.setWindowSize(windowSize);

    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(windowSize.x, windowSize.y), "Sokoban Game");

    // Win Sprite
    sf::Texture winTexture;
    if (!winTexture.loadFromFile("youwin.png")) {
        std::cerr << "Failed to load win image!" << std::endl;
        return 1;
    }
    sf::Sprite winSprite(winTexture);
    winSprite.scale(0.65, 0.65);

    bool gameWon = false;

    // Main loop
while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::W:
                case sf::Keyboard::Up:
                    game.movePlayer(SB::Direction::Up);
                    break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    game.movePlayer(SB::Direction::Down);
                    break;
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    game.movePlayer(SB::Direction::Left);
                    break;
                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    game.movePlayer(SB::Direction::Right);
                    break;
                case sf::Keyboard::R:
                    game.loadLevelFromFile(lvlFile);  // Reset level
                    break;
                case sf::Keyboard::Escape:  // Added case for Escape key to exit window
                    if (gameWon) {
                        window.close();
                    }
                    break;
                default:
                    break;
            }
        }
    }
        window.clear();
        // window.draw(game);
        if (!gameWon) {
            window.draw(game);
        } else {
            // Draw win sprite
            window.draw(winSprite);
        }
        // Check if the game is won
        if (!gameWon && game.isWon()) {
            gameWon = true;
            std::cout << "Congratulations! You have won the game!" << std::endl;
        }
        window.display();
    }
}
