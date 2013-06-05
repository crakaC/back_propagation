#ifndef BackPropagation_H_20130504
#define BackPropagation_H_20130504

#include<vector>
#include<string>
//訓練データ
struct TrainingData{
	std::vector< double > input;
	std::vector< double > output;
	TrainingData(int i, int o);
	//TrainingData(){}
};

//パラメータ
struct Params{
	int num_learn, num_sample;//学習打ち切り回数、訓練データ数
	int num_input, num_hidden, num_output;//入力素子数、隠れ層素子数、出力素子数
	int num_hidden_layer;
	double sigmoid_gain, learning_coefficient;//シグモイド関数ゲイン、学習係数
	double interia_coefficient;
	double threshold_error;//許容誤差
	bool is_trained, is_empty;//学習済みか、訓練データは存在するか。
	Params();
};

class BackPropagation{
public:
	BackPropagation();
	std::vector< double > output( const std::vector< double >& input );
	void setTrainingData( const std::string filename );
	void learnBatch();
	void learnOnline();

	//setter & getter
	int getInputNum();

	void setHiddenNodesNum( int n );
	int  getHiddenNodesNum();

	void setHiddenLayerNum( int n );
	int getHiddenLayerNum( void );	

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
	void initialize();

	void updateStateForward();
	void updateStateBackword( const int i_sample );
	void calcBondsWeightVariation();
	void resetBondsWeightVariation();
	void optimizeBondsWeight();
	double checkError( const TrainingData& target );

	double dRand();
	double sigmoid( const double inputs, const double gain );

	std::vector< double > input_layer;
	std::vector< std::vector < double > > hidden_layer;
	std::vector< double > output_layer;

	std::vector< double > output_layer_back;
	std::vector< std::vector < double > > hidden_layer_back;

	typedef std::vector< std::vector< std::vector< double > > > Vector3d;
	Vector3d bonds_weight;
	Vector3d bonds_weight_variation;

	Params param;//各種パラメータ
	Params param_bk; //学習時の状態を保存する

	std::vector< TrainingData > target; //教師信号
};

#endif
