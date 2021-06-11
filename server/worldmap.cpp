#include "worldmap.hpp"

using namespace Netocracy;

void WorldMapSquare::setRiverDirection(Direction* riverDirection) {
	if(this->riverDirection) delete this->riverDirection;
	this->riverDirection = riverDirection;
}

void WorldMap::allocateBlankMap() {
	map_ = new WorldMapSquare[xDimension_ * yDimension_];
	for (int row = 0; row < yDimension_; ++row) {
		for (int column = 0; column < xDimension_; ++column) {
			map_[xDimension_* row + column].setElevation(0.0);
			map_[xDimension_* row + column].setCoordinates(column, row);
		}
	}
}

WorldMapSquare* WorldMap::getSquare(int x, int y) {
	return &(map_[xDimension_* y + x]);
}

WorldMapSquare* MapBuilder::fetchRandomLandSquare() {
	while(true) {
		int xCoordinate = rand() % this->xDimension;
		int yCoordinate = rand() % this->yDimension;
		WorldMapSquare* candidate = this->newMap->getSquare(xCoordinate, yCoordinate);
		if(candidate->getElevation() >= 0.0) return candidate;
	}
}

void MapBuilder::fillEdgesAndQueueThem(std::queue<WorldMapSquare*>& squareQueue) {
	for(int xi=0; xi<this->xDimension; ++xi) {
		WorldMapSquare* top = this->newMap->getSquare(xi, 0);
		WorldMapSquare* bottom = this->newMap->getSquare(xi, this->yDimension - 1);
		top->setElevation(-1.0); squareQueue.push(top);
		bottom->setElevation(-1.0); squareQueue.push(bottom);
	}
	for(int yi=1; yi<this->yDimension - 1; ++yi) {
		WorldMapSquare* left = this->newMap->getSquare(0, yi);
		WorldMapSquare* right = this->newMap->getSquare(this->xDimension - 1, yi);
		left->setElevation(-1.0); squareQueue.push(left);
		right->setElevation(-1.0); squareQueue.push(right);
	}
}

bool MapBuilder::tryFloodSquare(WorldMapSquare* worldMapSquare) {
	if(worldMapSquare->getElevation() >= 0.0) {
		float randomRoll = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0));
		if(randomRoll <= FLOOD_RATE) {
			worldMapSquare->setElevation(-1.0);
			return true;
		}
	}
	return false;
}

void MapBuilder::createOcean() {
	std::queue<WorldMapSquare*> squareQueue;
	this->fillEdgesAndQueueThem(squareQueue);
	std::set<WorldMapSquare*> visited;
	this->enqueuePeaks(squareQueue);
	visited.clear();
	while(!squareQueue.empty()) {
		WorldMapSquare* current = squareQueue.front();
		squareQueue.pop();
		if(!visited.contains(current)) {
			std::vector<WorldMapSquare*> adjascentSquares;
			this->getAdjascentSquares(current, adjascentSquares);
			for(WorldMapSquare* aSquare : adjascentSquares) {
				if(this->tryFloodSquare(aSquare)) {
					squareQueue.push(aSquare);
				}
			}
			visited.insert(current);
		}
	}
}

void MapBuilder::randomlyPlacePeaks() {
	int placed = 0;
	while(placed < NUMBER_OF_PEAKS) {
		WorldMapSquare* randomPeakPlace = this->fetchRandomLandSquare();
		if(randomPeakPlace->getElevation() == 0.0) {
			randomPeakPlace->setElevation(this->maxElevation);
			this->peaks.push_back(randomPeakPlace);
			placed += 1;
		}
	}
}

void MapBuilder::enqueuePeaks(std::queue<WorldMapSquare*>& squareQueue) {
	for(WorldMapSquare* peak : this->peaks) {
		squareQueue.push(peak);
	}
}

void MapBuilder::smoothPeaks() {
	std::queue<WorldMapSquare*> squareQueue;
	std::set<WorldMapSquare*> visited;
	for(int i=0; i<SMOOTH_ITERATIONS; ++i) {
		this->enqueuePeaks(squareQueue);
		visited.clear();
		while(!squareQueue.empty()) {
			WorldMapSquare* current = squareQueue.front();
			squareQueue.pop();
			std::vector<WorldMapSquare*> adjascentSquares;
			this->getAdjascentSquares(current, adjascentSquares);
			for(WorldMapSquare* aSquare : adjascentSquares) {
				this->transferMassRandom(current, aSquare);
				if(visited.find(aSquare) == visited.end()) {
					squareQueue.push(aSquare);
					visited.insert(aSquare);
				}
			}
		}
	}
}

void MapBuilder::getAdjascentSquares(WorldMapSquare* centerSquare, std::vector<WorldMapSquare*>& adjascentSquares) {
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

void MapBuilder::transferMassRandom(WorldMapSquare* origin, WorldMapSquare* destination) {
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
		WorldMapSquare* riverSourcePlace = this->fetchRandomLandSquare();
		this->riverSources.push_back(riverSourcePlace);
	}
}

Direction* MapBuilder::getDirectionFromSquareToAdjSquare(WorldMapSquare* srcSquare, WorldMapSquare* dstSquare) {
	char origin='X', destination='X';

	if(srcSquare->getX() == dstSquare->getX()) {
		if(srcSquare->getY() == dstSquare->getY() - 1) {
			origin = 'N';
			destination = 'S';
		}
		else if(srcSquare->getY() == dstSquare->getY() + 1) {
			origin = 'S';
			destination = 'N';
		}
	}
	else if(srcSquare->getY() == dstSquare->getY()) {
		if(srcSquare->getX() == dstSquare->getX() - 1) {
			origin = 'E';
			destination = 'W';
		}
		else if(srcSquare->getX() == dstSquare->getX() + 1) {
			origin = 'W';
			destination = 'E';
		}
	}
	Direction* newDir = new Direction();
	newDir->setDirection(origin, destination);
	return newDir;
}

WorldMapSquare* MapBuilder::setRiverFlowDirection(WorldMapSquare* riverSquare) {
	if(riverSquare->getElevation() <= 0.0) return riverSquare;
	std::vector<WorldMapSquare*> adjascentSquares;
	this->getAdjascentSquares(riverSquare, adjascentSquares);
	WorldMapSquare* minElevationSquare = adjascentSquares[0];
	for(WorldMapSquare* ms : adjascentSquares) {
		if(ms->getElevation() < minElevationSquare->getElevation()) minElevationSquare = ms;
	}
	if(minElevationSquare->getElevation() <= riverSquare->getElevation()) {
		char origin, destination;
		Direction* riverDirectionRS = this->getDirectionFromSquareToAdjSquare(riverSquare, minElevationSquare);
		Direction* riverDirectionMES = this->getDirectionFromSquareToAdjSquare(riverSquare, minElevationSquare);
		riverSquare->setRiverDirection(riverDirectionRS);
		minElevationSquare->setRiverDirection(riverDirectionMES);
		return minElevationSquare;
	}
	return riverSquare;
}

void MapBuilder::makeRivers() {
	for(WorldMapSquare* riverSourceSquare : this->riverSources) {
		WorldMapSquare* riverSquare = riverSourceSquare;
		WorldMapSquare* nextRiverSquare = this->setRiverFlowDirection(riverSquare);
		while(nextRiverSquare != riverSquare) {
			riverSquare = nextRiverSquare;
			nextRiverSquare = this->setRiverFlowDirection(riverSquare);
		}
	}
}

WorldMap MapBuilder::generateRandomMap(int seed, int xDimension, int yDimension, float maxElevation) {
	this->xDimension = xDimension;
	this->yDimension = yDimension;
	this->maxElevation = maxElevation;
	this->newMap = new WorldMap(xDimension, yDimension);

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
