#include "map.hpp"

using namespace Netocracy;

void Direction::setDirection(char origin, char destination) {
    char validSD[] = {'N', 'S', 'E', 'W'};
    char *foundO = std::find(std::begin(validSD), std::end(validSD), origin);
    char *foundD = std::find(std::begin(validSD), std::end(validSD), destination);
    if (foundO == std::end(validSD) or foundD == std::end(validSD)) {
        throw new InvalidCardinalDir();
    }
    this->origin_=origin;
    this->destination_=destination;
}

void Direction::getDirectionStr(char *directionStr) {
                std::ostringstream stringStream;
                stringStream << origin_ << '-' << destination_ << '\0';
				strcpy(directionStr, stringStream.str().c_str());
}