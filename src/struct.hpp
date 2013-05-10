
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
	TrainingData(){}
};

//パラメータ
struct Params{
	int num_learn, num_sample;//学習打ち切り回数、訓練データ数
	int num_input, num_hidden, num_output;//入力素子数、隠れ層素子数、出力素子数
	double s_gain, learning_coefficient;//シグモイド関数ゲイン、学習係数
	double interia_coefficient;
	double threshold_error;//許容誤差
	bool is_trained, is_empty;//学習済みか、訓練データは存在するか。
	Params();
};

#endif