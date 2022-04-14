#include "map.hpp"

using namespace Netocracy;

const std::string Direction::validCardinals = "NSEW";

bool Direction::isCardinalValid(char cardinal) {
    return(validCardinals.find(cardinal) != validCardinals.npos);
}

void Direction::setDirection(char origin, char destination) {
    if (!isCardinalValid(origin))
        throw InvalidCardinalDir(origin);
    this->origin_=origin;

    if(!isCardinalValid(destination))
        throw InvalidCardinalDir(destination);
    this->destination_=destination;
}

const char* InvalidCardinalDir::what() const throw(){
    return "Invalid Cardinal Direction";
}