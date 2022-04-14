#include "gtest/gtest.h"
#include "map.hpp"

using namespace Netocracy;

TEST(testMap, cardinalCheck) {
    Direction newDirection('N', 'S');
}

TEST(testMap, createNewDirection) {
    Direction newDirection('N', 'S');
}

TEST(testMap, modifyDirection) {
    Direction newDirection('N', 'S');
    newDirection.setDirection('S', 'N');
}

TEST(testMap, testWrongDirectionOrigin) {
    ASSERT_THROW(Direction('X', 'S'), InvalidCardinalDir);
}

TEST(testMap, testWrongDirectionDest) {
    ASSERT_THROW(Direction('N', 'Y'), InvalidCardinalDir);
}