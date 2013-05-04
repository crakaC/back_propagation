#include<vector>
#include<map>
#include"struct.h"
struct Net{
	std::vector< double >x, h, y;
	std::vector< double >h_back, y_back;
	std::map< Pair, double > w1, w2;

	Net(params p);
	void init_weight();
};
