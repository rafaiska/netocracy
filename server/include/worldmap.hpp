#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <vector>
#include <set>
#include <algorithm>
#include <cstring>

#define NUMBER_OF_PEAKS 3
#define NUMBER_OF_RIVER_SOURCES 6
#define SQUARE_SIZE 20
#define SMOOTH_ITERATIONS 70
#define TRANSFER_MAX_RATE 0.2
#define MAP_SIZE 25
#define FLOOD_RATE 0.33

namespace Netocracy {
	class Direction {
		private:
			char origin_;
			char destination_;
		
		public:
			void setDirection(char origin, char destination){this->origin_=origin;this->destination_=destination;}
			const char* getDirectionStr() {
				if(this->origin_ == 'N' && this->destination_ == 'S') return "N-S\0";
				else if(this->origin_ == 'S' && this->destination_ == 'N') return "S-N\0";
				else if(this->origin_ == 'E' && this->destination_ == 'W') return "E-W\0";
				else if(this->origin_ == 'W' && this->destination_ == 'E') return "W-E\0";
				else return "\0";
			}
	};

	class MapSquare {
		private:
			bool haveRiver=false;
			Direction* riverDirectionFrom;
			Direction* riverDirectionTo;
			float elevation;
			float minTemperature;
			float maxTemperature;
			int x_;
			int y_;
		
		public:
			MapSquare() {this->riverDirectionFrom = nullptr; this->riverDirectionTo = nullptr;}
			~MapSquare() {if(this->riverDirectionFrom) delete this->riverDirectionFrom; if(this->riverDirectionTo) delete this->riverDirectionTo;}
			void setRiverDirection(char origin, char destination, bool from);
			float getElevation() {return this->elevation;}
			void setElevation(float newElevation){this->elevation = newElevation;}
			void setHaveRiver(bool haveRiver){this->haveRiver = haveRiver;}
			bool getHaveRiver(){return this->haveRiver;}
			int getX(){return this->x_;}
			int getY(){return this->y_;}
			const char* getRiverDirectionStr(bool from);
			MapSquare(int x, int y): x_(x), y_(y){this->riverDirectionFrom = nullptr; this->riverDirectionTo = nullptr;}
	};
	
	class Map {
		private:
			int xDimension_;
			int yDimension_;
			MapSquare** map_;

		public:
			Map(int xDimension, int yDimension)
			: xDimension_(xDimension),
			yDimension_(yDimension)
			{}
			void allocateBlankMap();
			MapSquare* getSquare(int x, int y) {return &(this->map_[x][y]);}
	};

	class MapBuilder {
		private:
			int xDimension;
			int yDimension;
			float maxElevation;
			std::vector<MapSquare*> peaks;
			std::vector<MapSquare*> riverSources;
			Map* newMap;

			void createOcean();
			void fillEdgesAndQueueThem(std::queue<MapSquare*>& squareQueue);
			bool tryFloodSquare(MapSquare* mapSquare);
			void randomlyPlacePeaks();
			void smoothPeaks();
			void enqueuePeaks(std::queue<MapSquare*>& squareQueue);
			void getAdjascentSquares(MapSquare* centerSquare, std::vector<MapSquare*>& adjascentSquares);
			void transferMassRandom(MapSquare* current, MapSquare* aSquare);
			MapSquare* fetchRandomLandSquare();
			void randomlyPlaceRiverSources();
			MapSquare* setRiverFlowDirection(MapSquare* riverSquare);
			void makeRivers();
		public:
			Map generateRandomMap(int seed, int xDimension, int yDimension, float maxElevation);
			float getMaxElevation();
			void getDirectionFromSquareToSquare(MapSquare* srcSquare, MapSquare* dstSquare, char& origin, char& destination);
	};
}