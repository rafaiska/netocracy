#include "map.hpp"

using namespace Netocracy;

void Direction::getDirectionStr(char *directionStr) {
                std::ostringstream stringStream;
                stringStream << origin_ << '-' << destination_ << '\0';
				strcpy(directionStr, stringStream.str().c_str());
}