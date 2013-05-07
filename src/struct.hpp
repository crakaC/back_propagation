
#ifndef STRUCT_H_20130504
#define STRUCT_H_20130504

#include<vector>
#include<iostream>

//訓練データ
struct TrainingData{
	std::vector< double > input;
	std::vector< double > output;
	TrainingData(int i, int o){
		input = std::vector< double >(i);
		output = std::vector< double >(o);
	}
};

//パラメータ
struct Params{
	int num_learn, num_sample;//学習打ち切り回数、訓練データ数
	int num_input, num_hidden, num_output;//入力素子数、隠れ層素子数、出力素子数
	double s_gain, epsilon;//シグモイド関数ゲイン、学習重み
	double threshold_error;//許容誤差
	bool is_trained, is_empty;
	Params();
};

typedef std::pair< int,int > Pair;

#endif
