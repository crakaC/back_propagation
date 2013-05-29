#ifndef BackPropagation_H_20130504
#define BackPropagation_H_20130504

#include<vector>
#include<unordered_map>
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

class Node{
public:
	std::unordered_map< Node*, double > weight_from;
	std::unordered_map< Node*, double > weight_to;
	std::unordered_map< Node*, double > weight_partial_to;
	double value;
	double back_value;
	Node();
	void updateStateForward( const double gain );
	void updateStateBackword( const double gain );
	void calcPartial( const Params& param );
	void optimizeWeight( const Params& param );
	void resetPartial( void );
private:
	double dRand();//-1~1の乱数
	double sigmoid( const double inputs, const double gain );
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
	void updateNodesStateForward();
	void updateNodesStateBackword( const int iSample );
	void calcPartial();
	void optimizeBondsWeight();
	double checkError( const TrainingData& target );
	double dRand();

	std::vector< Node > input_nodes;
	std::vector< Node > output_nodes;
	std::vector< std::vector< Node > > hidden_nodes; 
	Node dummy_node; //閾値用
	Params param;//各種パラメータ
	Params param_bk; //学習時の状態を保存する

	std::vector< TrainingData > target; //教師信号
};

#endif
