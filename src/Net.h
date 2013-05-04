#ifndef NET_H_20130504
#define NET_H_20130504

#include<vector>
#include<map>
#include"struct.h"

class Net{
public:
	Net(params* p);
	std::vector< double > output(const std::vector< double >& input);
	void learn( const std::vector< TrainingData >& target );

private:
	void init_weight();
	void init_node();
	void update_y();
	void reverse( const TrainingData& target );
	void fix_weight();
	double check_error( const TrainingData& target );

	std::vector< double >x, h, y;
	std::vector< double >h_back, y_back;
	std::map< Pair, double > w1, w2;
	params* param;
};

#endif
