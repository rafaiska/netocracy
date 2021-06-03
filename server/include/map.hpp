#include <cstring>
#include <sstream>

namespace Netocracy {
	class Direction {
		private:
			char origin_;
			char destination_;
		
		public:
			void setDirection(char origin, char destination){this->origin_=origin;this->destination_=destination;}
			void getDirectionStr(char *directionStr);
	};

	class MapSquare {
		private:
			int x_;
			int y_;
		
		public:
			MapSquare() {}
			~MapSquare() {}
            MapSquare(int x, int y): x_(x), y_(y){}
			int getX(){return this->x_;}
			int getY(){return this->y_;}
	};
}