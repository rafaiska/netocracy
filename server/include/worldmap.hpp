#include "map.hpp"
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
	class WorldMapSquare: public MapSquare {
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
			WorldMapSquare() {this->riverDirectionFrom = nullptr; this->riverDirectionTo = nullptr;}
			~WorldMapSquare() {if(this->riverDirectionFrom) delete this->riverDirectionFrom; if(this->riverDirectionTo) delete this->riverDirectionTo;}
			void setRiverDirection(char origin, char destination, bool from);
			float getElevation() {return this->elevation;}
			void setElevation(float newElevation){this->elevation = newElevation;}
			void setHaveRiver(bool haveRiver){this->haveRiver = haveRiver;}
			bool getHaveRiver(){return this->haveRiver;}
			const char* getRiverDirectionStr(bool from);
			WorldMapSquare(int x, int y): x_(x), y_(y){this->riverDirectionFrom = nullptr; this->riverDirectionTo = nullptr;}
	};
	
	class WorldMap {
		private:
			int xDimension_;
			int yDimension_;
			WorldMapSquare** map_;

		public:
			WorldMap(int xDimension, int yDimension)
			: xDimension_(xDimension),
			yDimension_(yDimension)
			{}
			void allocateBlankMap();
			WorldMapSquare* getSquare(int x, int y) {return &(this->map_[x][y]);}
	};

	class MapBuilder {
		private:
			int xDimension;
			int yDimension;
			float maxElevation;
			std::vector<WorldMapSquare*> peaks;
			std::vector<WorldMapSquare*> riverSources;
			WorldMap* newMap;

			void createOcean();
			void fillEdgesAndQueueThem(std::queue<WorldMapSquare*>& squareQueue);
			bool tryFloodSquare(WorldMapSquare* mapSquare);
			void randomlyPlacePeaks();
			void smoothPeaks();
			void enqueuePeaks(std::queue<WorldMapSquare*>& squareQueue);
			void getAdjascentSquares(WorldMapSquare* centerSquare, std::vector<WorldMapSquare*>& adjascentSquares);
			void transferMassRandom(WorldMapSquare* current, WorldMapSquare* aSquare);
			WorldMapSquare* fetchRandomLandSquare();
			void randomlyPlaceRiverSources();
			WorldMapSquare* setRiverFlowDirection(WorldMapSquare* riverSquare);
			void makeRivers();
		public:
			WorldMap generateRandomMap(int seed, int xDimension, int yDimension, float maxElevation);
			float getMaxElevation();
			void getDirectionFromSquareToSquare(WorldMapSquare* srcSquare, WorldMapSquare* dstSquare, char& origin, char& destination);
	};
}