#include "map.hpp"
#include <iostream>
#include <cstdlib>
#include <queue>
#include <vector>
#include <set>
#include <algorithm>
#include <cstring>

#define NUMBER_OF_PEAKS 50
#define NUMBER_OF_RIVER_SOURCES 50
#define SQUARE_SIZE 20
#define SMOOTH_ITERATIONS 30
#define TRANSFER_MAX_RATE 0.3
#define MAP_SIZE 40
#define FLOOD_RATE 0.45

namespace Netocracy {
	class WorldMapSquare: public MapSquare {
		private:
			bool riverSource=false;
			Direction* riverDirection;
			float elevation;
			float minTemperature;
			float maxTemperature;
		
		public:
			WorldMapSquare() {riverDirection = nullptr;}
			~WorldMapSquare() {if(riverDirection) delete riverDirection;}
			void setRiverDirection(Direction* riverDirection);
			float getElevation() {return this->elevation;}
			void setElevation(float newElevation){this->elevation = newElevation;}
			bool haveRiver(){return riverDirection;}
			Direction* getRiverDirection() {return riverDirection;}
	};
	
	class WorldMap {
		private:
			int xDimension_;
			int yDimension_;
			WorldMapSquare* map_;

		public:
			WorldMap(int xDimension, int yDimension)
			: xDimension_(xDimension),
			yDimension_(yDimension)
			{}
			~WorldMap(){if(map_) delete[] map_;}
			void allocateBlankMap();
			WorldMapSquare* getSquare(int x, int y);
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
			~MapBuilder(){if(newMap) delete newMap;}
			WorldMap generateRandomMap(int seed, int xDimension, int yDimension, float maxElevation);
			float getMaxElevation();
			Direction* getDirectionFromSquareToAdjSquare(WorldMapSquare* srcSquare, WorldMapSquare* dstSquare);
	};
}