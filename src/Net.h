#ifndef NET_H_20130504
#define NET_H_20130504

#include<vector>
#include<map>
#include"struct.h"

class Net{
public:
	Net( Params* p );
	std::vector< double > output( const std::vector< double >& input );
	void learn( const std::vector< TrainingData >& target );

private:
	void init_weight();
	void init_node();
	void update_y();
	void reverse( const TrainingData& target );
	void fix_weight();
	double check_error( const TrainingData& target );
	double sigmoid( double s );
	double d_rand();//-1~1の乱数


	std::vector< double >x, h, y;
	std::vector< double >h_back, y_back;
	std::map< Pair, double > w1, w2;
	Params* param;//各種パラメータ
	Params param_bk; //学習時の状態を保存する

	//std::vector< TrainingData > target; //教師信号
};

#endif
