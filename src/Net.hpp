#ifndef NET_H_20130504
#define NET_H_20130504

#include<vector>
#include<map>
#include"struct.hpp"

class Net{
public:
	Net();
	std::vector< double > output( const std::vector< double >& input );
	void set_training_data( const std::string filename );
	void learn();
	void learn_online();
	void execute();

	//setter & getter
	int get_input_num();

	void set_hidden_nodes_num( int n );
	int  get_hidden_nodes_num();

	void set_learn_num( int n );
	int get_learn_num();

	void set_threshold_error( double err );
	double get_threshold_error();

	void set_s_gain( double gain );
	double get_s_gain();

	void set_epsilon( double eps );
	double get_epsilon();

	bool is_trained();

private:
	void init_weight();
	void init_node();
	void update_y();
	void update_y( int isample );
	void reverse( const TrainingData& target );
	void fix_weight();
	double check_error( const TrainingData& target );
	double sigmoid( double s );
	double d_rand();//-1~1の乱数

	std::vector< double >x, h, y;
	std::vector< double >h_back, y_back;
	std::vector< std::vector< double > > xs, hs, ys;
	std::vector< std::vector< double > > h_backs, y_backs;
	std::map< Pair, double > w1, w2;

	Params param;//各種パラメータ
	Params param_bk; //学習時の状態を保存する

	std::vector< TrainingData > target; //教師信号
};

#endif
