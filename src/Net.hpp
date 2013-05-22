#ifndef NET_H_20130504
#define NET_H_20130504

#include<vector>
#include<map>
#include<string>

//訓練データ
struct TrainingData{
	static const int hoge = 100;
	std::vector< double > input;
	std::vector< double > output;
	TrainingData(int i, int o);
	//TrainingData(){}
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

class Net{
public:
	Net();
	std::vector< double > output( const std::vector< double >& input );
	void setTrainingData( const std::string filename );
	void learnBatch();
	void learnOnline();

	//setter & getter
	int getInputNum();

	void setHiddenNodesNum( int n );
	int  getHiddenNodesNum();

	void setLearnNum( int n );
	int getLearnNum();

	void setThresholdError( double err );
	double getThresholdError();

	void setSigmoidGain( double gain );
	double getSigmoidGain();

	void setLearningCoefficient( double eps );
	double getLearningCoefficient();

	bool isTrained();

private:
	void initWight();
	void initNode();
	void initLearn();
	void updateNodesState();
	void reverse( const int iSample );
	void calcPartial();
	void resetPartial();
	void optimizeWeightByBatch();
	void optimizeWeightOnline();
	double checkError( const TrainingData& target );
	double sigmoid( double s );
	double dRand();//-1~1の乱数

	std::vector< double >x, h, y;
	std::vector< double >h_back, y_back;
	std::vector< std::vector< double > > w1_partials, w2_partials, w1_inertia_term, w2_inertia_term;
	std::vector< std::vector< double > > w1, w2;

	Params param;//各種パラメータ
	Params param_bk; //学習時の状態を保存する

	std::vector< TrainingData > target; //教師信号
};

#endif
