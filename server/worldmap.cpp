#include "worldmap.hpp"

using namespace Netocracy;

void MapSquare::setRiverDirection(char origin, char destination, bool from) {
	this->haveRiver = true;
	//TODO: Validate origin, destination in {N, S, E, W}
	if(from) {
		this->riverDirectionFrom = new Direction();
		this->riverDirectionFrom->setDirection(origin, destination);
	}
	else {
		this->riverDirectionTo = new Direction();
		this->riverDirectionTo->setDirection(origin, destination);
	}
}

const char* MapSquare::getRiverDirectionStr(bool from) {
	if(from && this->riverDirectionFrom) return this->riverDirectionFrom->getDirectionStr();
	else if(!from && this->riverDirectionTo) return this->riverDirectionTo->getDirectionStr();
	else return "";
}

void Map::allocateBlankMap() {
	map_ = new MapSquare*[xDimension_];
	for (int row = 0; row < xDimension_; ++row) {
		map_[row] = new MapSquare[yDimension_];
		for (int column = 0; column < yDimension_; ++column) {
			map_[row][column] = MapSquare(row, column);
			map_[row][column].setElevation(0.0);
		}
	}
}

MapSquare* MapBuilder::fetchRandomLandSquare() {
	while(true) {
		int xCoordinate = rand() % this->xDimension;
		int yCoordinate = rand() % this->yDimension;
		MapSquare* candidate = this->newMap->getSquare(xCoordinate, yCoordinate);
		if(candidate->getElevation() >= 0.0) return candidate;
	}
}

void MapBuilder::fillEdgesAndQueueThem(std::queue<MapSquare*>& squareQueue) {
	for(int xi=0; xi<this->xDimension; ++xi) {
		MapSquare* top = this->newMap->getSquare(xi, 0);
		MapSquare* bottom = this->newMap->getSquare(xi, this->yDimension - 1);
		top->setElevation(-1.0); squareQueue.push(top);
		bottom->setElevation(-1.0); squareQueue.push(bottom);
	}
	for(int yi=1; yi<this->yDimension - 1; ++yi) {
		MapSquare* left = this->newMap->getSquare(0, yi);
		MapSquare* right = this->newMap->getSquare(this->xDimension - 1, yi);
		left->setElevation(-1.0); squareQueue.push(left);
		right->setElevation(-1.0); squareQueue.push(right);
	}
}

bool MapBuilder::tryFloodSquare(MapSquare* mapSquare) {
	if(mapSquare->getElevation() >= 0.0) {
		float randomRoll = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0));
		if(randomRoll <= FLOOD_RATE) {
			mapSquare->setElevation(-1.0);
			return true;
		}
	}
	return false;
}

void MapBuilder::createOcean() {
	std::queue<MapSquare*> squareQueue;
	this->fillEdgesAndQueueThem(squareQueue);
	std::set<MapSquare*> visited;
	this->enqueuePeaks(squareQueue);
	visited.clear();
	while(!squareQueue.empty()) {
		MapSquare* current = squareQueue.front();
		squareQueue.pop();
		std::vector<MapSquare*> adjascentSquares;
		this->getAdjascentSquares(current, adjascentSquares);
		for(MapSquare* aSquare : adjascentSquares) {
			if(this->tryFloodSquare(aSquare)) {
				squareQueue.push(aSquare);
			}
		}
	}
}

void MapBuilder::randomlyPlacePeaks() {
	int placed = 0;
	while(placed < NUMBER_OF_PEAKS) {
		MapSquare* randomPeakPlace = this->fetchRandomLandSquare();
		if(randomPeakPlace->getElevation() == 0.0) {
			randomPeakPlace->setElevation(this->maxElevation);
			this->peaks.push_back(randomPeakPlace);
			placed += 1;
		}
	}
}

void MapBuilder::enqueuePeaks(std::queue<MapSquare*>& squareQueue) {
	for(MapSquare* peak : this->peaks) {
		squareQueue.push(peak);
	}
}

void MapBuilder::smoothPeaks() {
	std::queue<MapSquare*> squareQueue;
	std::set<MapSquare*> visited;
	for(int i=0; i<SMOOTH_ITERATIONS; ++i) {
		this->enqueuePeaks(squareQueue);
		visited.clear();
		while(!squareQueue.empty()) {
			MapSquare* current = squareQueue.front();
			squareQueue.pop();
			std::vector<MapSquare*> adjascentSquares;
			this->getAdjascentSquares(current, adjascentSquares);
			for(MapSquare* aSquare : adjascentSquares) {
				this->transferMassRandom(current, aSquare);
				if(visited.find(aSquare) == visited.end()) {
					squareQueue.push(aSquare);
					visited.insert(aSquare);
				}
			}
		}
	}
}

void MapBuilder::getAdjascentSquares(MapSquare* centerSquare, std::vector<MapSquare*>& adjascentSquares) {
	int centerX = centerSquare->getX();
	int centerY = centerSquare->getY();
	if(centerX < this->xDimension - 1) {
		adjascentSquares.push_back(this->newMap->getSquare(centerX + 1, centerY));
	}
	if(centerX > 0) {
		adjascentSquares.push_back(this->newMap->getSquare(centerX - 1, centerY));
	}
	if(centerY < this->yDimension - 1) {
		adjascentSquares.push_back(this->newMap->getSquare(centerX, centerY + 1));
	}
	if(centerY > 0) {
		adjascentSquares.push_back(this->newMap->getSquare(centerX, centerY - 1));
	}
}

void MapBuilder::transferMassRandom(MapSquare* origin, MapSquare* destination) {
	if(destination->getElevation() < origin->getElevation()) {
		float transferRate = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/TRANSFER_MAX_RATE));
		float transferMass = origin->getElevation() * transferRate;
		origin->setElevation(origin->getElevation() - transferMass);
		if(destination->getElevation() >= 0.0)
			destination->setElevation(destination->getElevation() + transferMass);
		// std::cout << "Transfered " << transferMass << " from " 
		// 	<< origin->getX() << ", " << origin->getY() << " to "
		// 	<< destination->getX() << ", " << destination->getY() << '\n';
	}
}

void MapBuilder::randomlyPlaceRiverSources() {
	for(int i=0; i<NUMBER_OF_RIVER_SOURCES; ++i) {
		MapSquare* riverSourcePlace = this->fetchRandomLandSquare();
		riverSourcePlace->setHaveRiver(true);
		this->riverSources.push_back(riverSourcePlace);
	}
}

void MapBuilder::getDirectionFromSquareToSquare(MapSquare* srcSquare, MapSquare* dstSquare, char& origin, char& destination) {
	if(srcSquare->getX() == dstSquare->getX()) {
		if(srcSquare->getY() < dstSquare->getY()) {
			origin = 'N';
			destination = 'S';
		}
		else if(srcSquare->getY() > dstSquare->getY()) {
			origin = 'S';
			destination = 'N';
		}
	}
	else if(srcSquare->getY() == dstSquare->getY()) {
		if(srcSquare->getX() < dstSquare->getX()) {
			origin = 'W';
			destination = 'E';
		}
		else if(srcSquare->getX() > dstSquare->getX()) {
			origin = 'E';
			destination = 'W';
		}
	}
}

MapSquare* MapBuilder::setRiverFlowDirection(MapSquare* riverSquare) {
	if(riverSquare->getElevation() <= 0.0) return riverSquare;
	std::vector<MapSquare*> adjascentSquares;
	this->getAdjascentSquares(riverSquare, adjascentSquares);
	MapSquare* minElevationSquare = adjascentSquares[0];
	for(MapSquare* ms : adjascentSquares) {
		if(ms->getElevation() < minElevationSquare->getElevation()) minElevationSquare = ms;
	}
	if(minElevationSquare->getElevation() <= riverSquare->getElevation()) {
		char origin, destination;
		this->getDirectionFromSquareToSquare(riverSquare, minElevationSquare, origin, destination);
		riverSquare->setRiverDirection(origin, destination, false);
		minElevationSquare->setRiverDirection(destination, origin, true);
		return minElevationSquare;
	}
	return riverSquare;
}

void MapBuilder::makeRivers() {
	for(MapSquare* riverSourceSquare : this->riverSources) {
		MapSquare* riverSquare = riverSourceSquare;
		MapSquare* nextRiverSquare = this->setRiverFlowDirection(riverSquare);
		while(nextRiverSquare != riverSquare) {
			riverSquare = nextRiverSquare;
			nextRiverSquare = this->setRiverFlowDirection(riverSquare);
		}
	}
}

Map MapBuilder::generateRandomMap(int seed, int xDimension, int yDimension, float maxElevation) {
	this->xDimension = xDimension;
	this->yDimension = yDimension;
	this->maxElevation = maxElevation;
	this->newMap = new Map(xDimension, yDimension);

	srand(seed);
	newMap->allocateBlankMap();
	std::cout << "Creating ocean...\n";
	this->createOcean();
	std::cout << "Placing peaks...\n";
	this->randomlyPlacePeaks();
	std::cout << "Smoothing map...\n";
	this->smoothPeaks();
	std::cout << "Placing river sources...\n";
	this->randomlyPlaceRiverSources();
	std::cout << "Making rivers...\n";
	this->makeRivers();
	// this->generateTemperatures();
	return *(this->newMap);
}

float MapBuilder::getMaxElevation() {
	std::vector<float> elevations;
	for(int xi=0; xi<this->xDimension; ++xi)
		for(int yi=0; yi<this->yDimension; ++yi)
			elevations.push_back(this->newMap->getSquare(xi, yi)->getElevation());
	return *std::max_element(elevations.begin(), elevations.end());
}
