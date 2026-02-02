// Copyright 2024 Ashton Roxas
#include <iostream>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "Sokoban.hpp"

using SB::Sokoban;

BOOST_AUTO_TEST_CASE(testSize) {
    Sokoban sokoban;
    std::ifstream file("level1.lvl");
    file >> sokoban;
    file.close();
    BOOST_REQUIRE_EQUAL(sokoban.pixelWidth(), sokoban.pixelWidth());
    BOOST_REQUIRE_EQUAL(sokoban.pixelHeight(), sokoban.pixelHeight());
}

BOOST_AUTO_TEST_CASE(testMovePlayer) {
    Sokoban sokoban("level1.lvl");
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sf::Vector2i _location = sokoban.playerLoc();
    BOOST_REQUIRE_EQUAL(_location.x, 7);
    BOOST_REQUIRE_EQUAL(_location.y, 6);
}

BOOST_AUTO_TEST_CASE(testMovePlayerVSBoxes) {
    Sokoban sokoban("level2.lvl");
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Up);
    sf::Vector2i _location = sokoban.playerLoc();
    BOOST_REQUIRE_EQUAL(_location.x, 8);
    BOOST_REQUIRE_EQUAL(_location.y, 5);
}

BOOST_AUTO_TEST_CASE(testlotsBoxes) {
    Sokoban sokoban("lotsBoxes.lvl");
    sokoban.movePlayer(SB::Direction::Down);
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Left);
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Up);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Right);
    sokoban.movePlayer(SB::Direction::Down);
    sokoban.movePlayer(SB::Direction::Left);
    BOOST_REQUIRE(sokoban.isWon());
}
