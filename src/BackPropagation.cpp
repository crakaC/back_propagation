/*****************************************************************************/
/*! @file	BackPropagation.cpp
	@brief	BackPropagationの実装部分

	BackPropagationに関わる部分の実装

	@sa	BackPropagation.hpp	ヘッダ
	@sa	BackPropagation_setter_and_getter.cpp setter および getter の実装部分
	@par	更新履歴
		- 2012/05/04 Ver. 1.0
			- 基本的な機能の実装 (by Kosuke Shirakashi)
		- 2012/06/09 Ver. 1.1
			- 中間層数および中間層素子数を調整可能に (by Kosuke Shirakashi)
	@par Copyright
		&copy; 2013　SHIMA Lab. 
******************************************************************************/

#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<fstream>
#include<iostream>
#include<ctime>
#include<algorithm>

#include"BackPropagation.hpp"

TrainingData::TrainingData(int i, int o){
	input = std::vector< double >(i);
	output = std::vector< double >(o);
}

Params::Params()
{
		num_input = num_hidden = num_output = 0;
		num_learn = 50000;
		num_hidden = 10;
		num_hidden_layer = 2;
		sigmoid_gain = 1.0;
		learning_coefficient = 0.07;
		interia_coefficient = 0.05;
		threshold_error = 0.001;
		is_empty = true;
		is_trained = false;
}

BackPropagation::BackPropagation()
{
	//const int MAX_INPUT = 256;
	//const int MAX_OUTPUT = 256;
	const int MAX_HIDDEN = 256;
	const int MAX_HIDDEN_LAYER = 256;

	//input_layer.reserve( MAX_INPUT );
	hidden_layer.assign( MAX_HIDDEN_LAYER, std::vector< double >( MAX_HIDDEN + 1 ) );
	//output_layer.reserve( MAX_OUTPUT );
	hidden_layer_back.assign( MAX_HIDDEN_LAYER, std::vector< double >( MAX_HIDDEN + 1 ) );
	//output_layer_back.reserve( MAX_OUTPUT );
	//bonds_weight.reserve( sizeof(double) * (MAX_HIDDEN_LAYER + 1) * (MAX_HIDDEN + 1) * MAX_HIDDEN );
	//bonds_weight_variation.reserve( sizeof(double) * (MAX_HIDDEN_LAYER + 1) * (MAX_HIDDEN + 1) * MAX_HIDDEN );
}

void BackPropagation::setTrainingData( const std::string filename = "training2.dat" )
{
	using namespace std;

	unsigned int size;
	ifstream ifs;

	ifs.open( filename.c_str() );
	//ファイルを開けなかったら戻る。
	if ( !ifs.is_open() ) {
		cout << "cannot open " << filename << endl;
	} else {
		//データからパラメータを読み込む
		ifs >> param.num_sample >> param.num_input >> param.num_output;
		if ( param.num_sample < 1 || param.num_input < 1 || param.num_output < 1 ) {
			cout<< "file format is wrong." <<endl;
		} else {
			param.is_empty = false;
			param.is_trained = false;

			//initialize target
			target.assign( param.num_sample, TrainingData( param.num_input, param.num_output ) );
			//訓練データをtargetに格納
			for ( int isample = 0; isample < param.num_sample ; isample++ ) {
				for ( int i = 0; i < param.num_input; i++ ) {
					ifs >> target[isample].input[i];
				}
				for ( int i = 0; i < param.num_output; i++ ) {
					ifs >> target[isample].output[i];
				}
			}

			//訓練データを表示
			printf( "***** input training data from %s *****\n", filename.c_str() );
			for ( int isample = 0; isample < param.num_sample; isample++ ) {
				printf( "訓練データ No.%d\n", isample + 1 );
				printf( "入力：" );
				size = target[isample].input.size();
				for ( unsigned int i = 0; i < size; i++ ) {
					cout << target[isample].input[i];
					if ( i < size - 1 ) {
						cout << ' ';
					} else {
						//Not Reached
					}
				}
				printf( "\n出力：" );
				size = target[isample].output.size();
				for ( unsigned int i = 0; i < size; i++ ) {
					cout << target[isample].output[i];
					if( i < size - 1 ) {
						cout << ' ';
					} else {
						//Not Reached
					}
				}
				cout << endl;
			}
			printf("*****************************************\n");
		}
	}
}

//与えられた入力データを用いて出力データを返す．
std::vector< double > BackPropagation::output( const std::vector< double >& input )
{
	std::vector< double > output( param_bk.num_output );

	input_layer = input;
	
	updateStateForward();

	return output_layer;
}


void BackPropagation::learnBatch()
{
	int ilearn, isample;//iterator for learning count, sample number
	double error = 0, max_error = 0;
	char buf[1024];//buffer for output log.
	std::ofstream ofs("train_batch.log");
	std::ofstream max_error_log("max_error_batch.log");
	clock_t begin, end;

	if( param.is_empty ){
		printf( "先に訓練データを入力してください\n" );
	} else {
		initialize();
		sprintf( buf, "hidden_nodes = %d, gain = %lf, learning_coefficient = %lf, threshold_error = %G\n", param.num_hidden, param.sigmoid_gain, param.learning_coefficient, param.threshold_error );
		ofs << buf;

		/**************/
		begin = clock();
		/**************/
		for( ilearn = 0; ilearn <= param.num_learn; ilearn++ ){
			max_error = 0;
			sprintf( buf, "\n学習回数 = %d\n", ilearn );
			ofs << buf;
			for( isample = 0 ; isample < param.num_sample; isample++ ){
				//順方向の動作
				//入力値を訓練データからx(入力層素子)につっこむ
				input_layer = target[isample].input;
				//各層の素子の状態を更新
				updateStateForward();

				//誤差の評価
				error = checkError( target[isample] );
				max_error = std::max( error, max_error );

				//output log
				for( int j = 0; j < param.num_output; j++ ){
					sprintf( buf, "Data NO.%d, 誤差 = %G (目標:%f 出力:%f)\n", isample+1, pow( ( target[isample].output[j] -output_layer[j] ), 2 ), target[isample].output[j], output_layer[j]);
					ofs << buf;

				}

				//逆方向に動作させる
				updateStateBackword( isample );
				calcBondsWeightVariation();
			}
			optimizeBondsWeight();

			sprintf( buf, "%G\n", max_error);
			max_error_log << buf;
			//printf("%5d, Max error = %G\r", ilearn, max_error);

			//誤差が小さくなったらループを抜ける。
			if( max_error < param.threshold_error ){
				break;
			} else {
				//Not Reached.
			}
		}
		/**************/
		end = clock();
		/**************/

		printf( "学習回数:%d, 誤差:%G (%lf sec)\n",ilearn - 1, max_error, (double)(end - begin)/CLOCKS_PER_SEC );
		param.is_trained = true;
	}
}

//online learning mode
void BackPropagation::learnOnline()
{
	int ilearn, isample;

	char buf[1024];
	std::ofstream ofs("train_online.log");
	std::ofstream error_log("max_error_online.log");
	double error = 0, max_error = 0;
	clock_t begin, end;

	if( param.is_empty ){
		printf("input training data first.\n");
	} else {
		initialize();

		begin = clock();

		for( ilearn = 0; ilearn <= param.num_learn; ilearn++ ){
			max_error = 0;
			sprintf( buf, "\n学習回数 = %d\n", ilearn );
			ofs << buf;
			for( isample = 0; isample < param.num_sample; isample++ ){

				//入力値を訓練データから入力層ノードにつっこむ
				input_layer = target[isample].input;
				//入力から出力の値を計算
				updateStateForward();
				//誤差の評価
				error = checkError( target[isample] );//checkNodesError( target[isample] );
				max_error = std::max( error, max_error );

				//logの生成
				for( int j = 0; j < param.num_output; j++ ){
					sprintf( buf, "Data NO.%d, 誤差 = %G (目標:%f 出力:%f)\n", isample+1, error, target[isample].output[j], output_layer[j]);
					ofs << buf;
				}
				//逆方向の動作
				updateStateBackword( isample );
				//重みの修正
				calcBondsWeightVariation();
				optimizeBondsWeight();
			}
			error_log << max_error << std::endl;
			//printf("%5d, Max error = %G\r", ilearn, max_error);
			//誤差が小さくなったらループを抜ける。
			if( max_error < param.threshold_error ){
				break;
			} else {
				//Not Reached.
			}
		}
		end = clock();
		printf( "学習回数:%d, 誤差:%G (%lf sec)\n",ilearn - 1, max_error, (double)(end - begin)/CLOCKS_PER_SEC );
		param.is_trained = true;
	}
}

//誤差のチェック
double BackPropagation::checkError( const TrainingData& target )
{
	double error = 0;
	for( int j = 0; j < param.num_output; j++ ){
		error += pow( (target.output[j] - output_layer[j]), 2 );
	}
	error /=  static_cast<double>( param.num_output );
	return error;
}


//各素子を初期化
void BackPropagation::initialize()
{
	input_layer.resize( param.num_input + 1 );
	hidden_layer.resize( param.num_hidden_layer, std::vector< double >( param.num_hidden + 1 ) );
	output_layer.resize( param.num_output );

	hidden_layer_back.resize( param.num_hidden_layer, std::vector< double >( param.num_hidden + 1 ) );
	output_layer_back.resize( param.num_output );

	bonds_weight.resize( param.num_hidden_layer + 1 );
	bonds_weight_variation.resize( param.num_hidden_layer + 1 );
	//between input_layer and hidden_layer
	bonds_weight[0].assign( param.num_input + 1, std::vector< double >( param.num_hidden ) );
	bonds_weight_variation[0].assign( param.num_input + 1, std::vector< double >( param.num_hidden, 0.0 ) );
	for( int i = 0; i < param.num_input + 1; i++ ) {
		for( int j = 0; j < param.num_hidden; j++ ) {
			bonds_weight[0][i][j] = (i == param.num_input) ? 1.0 : dRand();
		}
	}
	//between hidden_layer and hidden_layer
	for( int layer = 1; layer < param.num_hidden_layer; layer++ ) {
		bonds_weight[layer].assign( param.num_hidden + 1, std::vector< double >( param.num_hidden, 0.0 ) );
		bonds_weight_variation[layer].assign( param.num_hidden + 1, std::vector< double >( param.num_hidden, 0.0 ) );
		for( int i = 0; i < param.num_hidden + 1; i++ ) {
			for( int j = 0; j < param.num_hidden; j++ ) {
				bonds_weight[layer][i][j] = (i == param.num_hidden) ? 1.0 : dRand();
			}
		}
	}
	//between hidden_layer and output_layer
	bonds_weight[param.num_hidden_layer].assign( param.num_hidden + 1, std::vector< double >( param.num_output ) );
	bonds_weight_variation[param.num_hidden_layer].assign( param.num_hidden + 1, std::vector< double >( param.num_output, 0.0 ) );
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			bonds_weight[param.num_hidden_layer][i][j] = (i == param.num_hidden) ? 1.0 : dRand();			
		}
	}

	//設定を記憶
	param_bk = param;
	printf( "hidden_layers = %d, hidden_nodes = %d, gain = %lf, learning_coefficient = %lf, threshold_error = %G\n", param.num_hidden_layer, param.num_hidden, param.sigmoid_gain, param.learning_coefficient, param.threshold_error );
}

void BackPropagation::updateStateForward()
{
	double total_input;
	//input -> hidden
	for( int i = 0; i < param_bk.num_hidden ; i++ ){
		total_input = 0.0;
		for( int j = 0; j < param_bk.num_input + 1; j++ ){
			total_input += bonds_weight[0][j][i] * input_layer[j];
		}
		hidden_layer[0][i] = sigmoid( total_input, param.sigmoid_gain );
	}

	//hidden layer -> hidden layer ( when num_hidden_layer >= 2 )
	for( int l = 1; l < param_bk.num_hidden_layer; l++ ) {
		for( int i = 0; i < param_bk.num_hidden; i++ ) {
			total_input = 0.0;
			for( int j = 0; j < param_bk.num_hidden + 1; j++ ){
				total_input += bonds_weight[l][j][i] * hidden_layer[l - 1][j];
			}
			hidden_layer[l][i] = sigmoid( total_input, param_bk.sigmoid_gain );
		}
	}

	//hidden_layer -> output_layer
	for( int i = 0; i < param_bk.num_output; i++ ) {
		total_input = 0.0;
		for( int j = 0; j < param_bk.num_hidden + 1; j++ ){
			total_input += bonds_weight[param_bk.num_hidden_layer][j][i] * hidden_layer[param_bk.num_hidden_layer - 1][j];
		}
		output_layer[i] = sigmoid( total_input, param_bk.sigmoid_gain );
	}
}

void BackPropagation::updateStateBackword( const int i_sample )
{
	TrainingData target = this->target[i_sample];
	double total_input;
	int last_layer = param.num_hidden_layer - 1;

	for( int i = 0; i < param.num_output; i++ ){
		output_layer_back[i] = (output_layer[i] - target.output[i]) * (1.0 - output_layer[i]) * output_layer[i] * param.sigmoid_gain;
	}

	//hidden layer <-> output layer
	for( int i = 0; i < param.num_hidden; i++ ){
		total_input = 0.0;
		for( int j = 0; j < param.num_output; j++ ){
			total_input += bonds_weight[param.num_hidden_layer][i][j] * output_layer_back[j];
		}
		hidden_layer_back[last_layer][i] = total_input * (1.0 - hidden_layer[last_layer][i]) * hidden_layer[last_layer][i] * param.sigmoid_gain;
	}

	//hidden layer <-> hidden_layer
	for( int layer = last_layer - 1; layer >  0; layer-- ){
		for( int i = 0; i < param.num_hidden; i++ ){
			total_input = 0;
			for(int j = 0; j < param.num_hidden; j++){
				total_input += bonds_weight[layer + 1][i][j] * hidden_layer_back[layer + 1][j];
			}
			hidden_layer_back[layer][i] = total_input * (1.0 - hidden_layer[layer][i]) * hidden_layer[layer][i] * param.sigmoid_gain;
		}
	}
}

void BackPropagation::calcBondsWeightVariation()
{
	//input <- hidden
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			bonds_weight_variation[0][i][j] -= param.learning_coefficient * input_layer[i] * hidden_layer_back[0][j];
		}
	}

	//hidden <- hidden
	for( int l = 1; l < param.num_hidden_layer; l++ ){
		for( int i = 0; i < param.num_hidden + 1; i++ ){
			for( int j = 0; j < param.num_hidden; j++ ){
				bonds_weight_variation[l][i][j] -= param.learning_coefficient * hidden_layer[l-1][i] * hidden_layer_back[l][j];
			}
		}
	}

	//hidden <- output
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			bonds_weight_variation[param.num_hidden_layer][i][j] -= param.learning_coefficient * hidden_layer[param.num_hidden_layer - 1][i] * output_layer_back[j];
		}
	}
}

void BackPropagation::resetBondsWeightVariation()
{
	/*
	for( int i = 0; i < param.num_input + 1; i++ ){
		std::fill_n( bonds_weight_variation[0][i], param.num_hidden, 0.0 );
	}
	for( int l = 1; l < param.num_hidden_layer; l++ ){
		for( int i = 0; i < param.num_hidden + 1; i++ ){
			std::fill_n( bonds_weight_variation[l][i], param.num_hidden, 0.0 );
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		std::fill_n( bonds_weight_variation[param.num_hidden_layer][i], param.num_output, 0.0 );
	}
	*/
	for( auto ite = bonds_weight_variation.begin(); ite != bonds_weight_variation.end(); ite++){
		for( auto jte = (*ite).begin(); jte != (*ite).end(); jte++ ){
			std::fill( (*jte).begin(), (*jte).end(), 0.0);
		}
	}

}

void BackPropagation::optimizeBondsWeight()
{
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			bonds_weight[0][i][j] += bonds_weight_variation[0][i][j];
		}
	}
	for( int l = 1; l < param.num_hidden_layer; l++ ){
		for( int i = 0; i < param.num_hidden + 1; i++ ){
			for( int j = 0; j < param.num_hidden; j++ ){
				bonds_weight[l][i][j] += bonds_weight_variation[l][i][j];
			}
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			bonds_weight[param.num_hidden_layer][i][j] += bonds_weight_variation[param.num_hidden_layer][i][j];
		}
	}
	resetBondsWeightVariation();
}


//-1から1の範囲の乱数を返す。
double BackPropagation::dRand()
{
	double ret = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
	return ( rand()%2 == 0 ) ? ret : -ret;
}

double BackPropagation::sigmoid( const double inputs, const double gain )
{
	return 1 / ( 1.0 + exp( gain * (-inputs) ) );
}
