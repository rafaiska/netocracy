#include <string>
#include <sstream>
#include <algorithm>
#include <exception>
#include <vector>

namespace Netocracy {
	class Direction {
		private:
			char origin_;
			char destination_;
			static const std::string validCardinals;
		
		public:
			Direction(char origin, char destination){setDirection(origin, destination);}
			static bool isCardinalValid(char cardinal);
			void setDirection(char origin, char destination);
			char getOrigin(){return origin_;}
			char getDestination(){return destination_;}
	};

	class MapSquareObject {
		std::string id;
	};

	enum class TerrainType {
		WATER,
		GRASS,
		SAND,
		ROCK
	};

	class MapSquare {
		public:
		int x_;
		int y_;
		TerrainType terrainType;
		std::vector<MapSquareObject> objects;
	};

	class InvalidCardinalDir: public std::exception{
		char invalidDirection_;

		public:
		InvalidCardinalDir(char invalidDirection): invalidDirection_(invalidDirection){}
		const char* what() const throw();
	};
}