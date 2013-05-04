
#ifndef STRUCT_H_20130504
#define STRUCT_H_20130504

#include<vector>
#include<iostream>

//訓練データ
struct TrainingData{
	std::vector< int > input;
	std::vector< int > output;
	TrainingData(int i, int o){
		input = std::vector<int>(i);
		output = std::vector<int>(o);
	}
};

//パラメータ
class Params{
public:
	int num_learn, num_sample;//学習打ち切り回数、訓練データ数
	int num_input, num_hidden, num_output;//入力素子数、隠れ層素子数、出力素子数
	double s_gain, epsilon;//シグモイド関数ゲイン、学習重み
	double threshold_error;//許容誤差
	bool is_trained, is_empty;
	Params();
	Params( const Params& p );
	void set_all();
	void set_learn_num();
	void set_hidden_num();
	void set_s_gain();
	void set_epsilon();
	void set_threshold_error();
};

typedef std::pair< int,int > Pair;

#endif
