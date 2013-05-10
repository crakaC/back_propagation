#ifndef NET_H_20130504
#define NET_H_20130504

#include<vector>
#include<map>
#include"struct.hpp"


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
