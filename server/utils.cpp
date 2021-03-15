namespace Netocracy {
	class Direction {
		private:
			char source_;
			char destination_;
		public:
			Direction(char source, char destination)
			: source_(source), destination_(destination)
			{}
			~Direction(){}
	}
}
