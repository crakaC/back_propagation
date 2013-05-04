#ifndef NET_H_20130504
#define NET_H_20130504

#include<vector>
#include<map>
#include"struct.h"
struct Net{
	std::vector< double >x, h, y;
	std::vector< double >h_back, y_back;
	std::map< Pair, double > w1, w2;
	params* param;

	Net(params* p);
	void init_weight();
	void init_node();
	std::vector< double > output(const std::vector< double >& input);
	void update_y();
	void reverse( const TrainingData& target );
	void learn( const std::vector< TrainingData >& target );
	void fix_weight();
	double check_error( const TrainingData& target );
};

#endif
