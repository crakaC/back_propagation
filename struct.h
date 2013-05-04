#ifndef STRUCT_H_20130504
#define STRUCT_H_20130504

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
struct params{
	int num_learn, num_sample;//学習打ち切り回数、訓練データ数
	int num_input, num_hidden, num_output;//入力素子数、隠れ層素子数、出力素子数
	double s_gain, epsilon;//シグモイド関数ゲイン、学習重み
	double threshold_error;//許容誤差
	params(){
		num_learn = 10000;
		num_hidden = 10;
		s_gain = 1.0;
		epsilon = 0.05;
		threshold_error = 0.01;
	}
};

#endif
