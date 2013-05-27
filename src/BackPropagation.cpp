#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<fstream>
#include<iostream>

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
		s_gain = 1.0;
		learning_coefficient = 0.07;
		interia_coefficient = 0.05;
		threshold_error = 0.00001;
		is_empty = true;
		is_trained = false;
}

BackPropagation::BackPropagation(){
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


//シグモイド関数
double BackPropagation::sigmoid( const double s ){
	return  1 / ( 1.0 + exp( param.s_gain * (-s) ) );
}

//-1から1の範囲の乱数を返す。
double BackPropagation::dRand(){
	double ret = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
	return ( rand()%2 == 0 ) ? ret : -ret;
}

//各素子を初期化
void BackPropagation::initNode()
{
	//学習用
	input_nodes.resize( param.num_input + 1 );
	hidden_nodes.resize( param.num_hidden + 1 );
	output_nodes.resize( param.num_output );
	h_back.resize( param.num_hidden );
	y_back.resize( param.num_output );

	//設定を記憶
	param_bk = param;
}

//結線の重みを初期化
void BackPropagation::initWight(){

	w1.assign( param.num_input + 1, std::vector< double >( param.num_hidden ) );
	w2.assign( param.num_hidden + 1, std::vector< double >( param.num_output ) );

	w1_partials.assign( param.num_input + 1, std::vector< double >( param.num_hidden, 0.0 ) );
	w2_partials.assign( param.num_hidden + 1, std::vector< double >( param.num_output, 0.0 ) );

	w1_inertia_term.assign( param.num_input + 1, std::vector< double >( param.num_hidden, 0.0 ) );
	w2_inertia_term.assign( param.num_hidden + 1, std::vector< double >( param.num_output, 0.0 ) );

	srand( time( NULL ) );
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			w1[i][j] = dRand();
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			w2[i][j] = dRand();
		}
	}
}

//与えられた入力データを用いて出力データを返す．
std::vector< double > BackPropagation::output( const std::vector< double >& input )
{
	double input_to_node;
	std::vector< double > output( param_bk.num_output );

	//隠れ素子の値を計算
	for( int j = 0; j < param_bk.num_hidden ; j++ ){
		input_to_node = 0;	
		for( int i = 0; i < param_bk.num_input + 1; i++ ){
			input_to_node += w1[i][j] * input[i];
		}
		hidden_nodes[j] = sigmoid( input_to_node );
	}

	//出力値の計算
	for( int j = 0; j < param_bk.num_output; j++ ){
		input_to_node = 0;
		for( int i = 0; i < param_bk.num_hidden + 1; i++ ){
			input_to_node += w2[i][j] *hidden_nodes[i];
		}
		output[j] = sigmoid( input_to_node );
	}

	return output;
}

//メンバのxから,yを更新する。
void BackPropagation::updateNodesStateForward()
{
	double input_to_node;
	//隠れ素子値の計算
	for( int j = 0; j < param.num_hidden; j++ ){
		input_to_node = 0;
		for( int i = 0; i < param.num_input + 1; i++ ){
			input_to_node += w1[i][j] *input_nodes[i];
		}
		hidden_nodes[j] = sigmoid( input_to_node );
	}

	//出力値の計算
	for( int j = 0; j < param.num_output; j++ ){
		input_to_node = 0;
		for( int i = 0; i < param.num_hidden + 1; i++ ){
			input_to_node += w2[i][j] *hidden_nodes[i];
		}
		output_nodes[j] = sigmoid( input_to_node );
	}
}

void BackPropagation::initLearn()
{
	printf( "hidden_nodes = %d, gain = %lf, learning_coefficient = %lf, threshold_error = %G\n", param.num_hidden, param.s_gain, param.learning_coefficient, param.threshold_error );
	initNode();
	initWight();
	return;
}

void BackPropagation::learnBatch()
{
	int ilearn, isample;//iterator for learning count, sample number
	double max_error = 0;
	char buf[1024];//buffer for output log.
	std::ofstream ofs("train_batch.log");
	std::ofstream max_error_log("max_error.log");
	//max_error_log << "" << std::endl;

	if( param.is_empty ){
		printf( "先に訓練データを入力してください\n" );
	} else {
		initLearn();//check training data and initialize Nodes and Weight
		sprintf( buf, "hidden_nodes = %d, gain = %lf, learning_coefficient = %lf, threshold_error = %G\n", param.num_hidden, param.s_gain, param.learning_coefficient, param.threshold_error );
		ofs << buf;
		for( ilearn = 0; ilearn < param.num_learn; ilearn++ ){
			max_error = 0;
			sprintf( buf, "\n学習回数 = %d\n", ilearn );
			ofs << buf;
			for( isample = 0 ; isample < param.num_sample; isample++ ){
				//順方向の動作
				//入力値を訓練データからx(入力層素子)につっこむ
				input_nodes = target[isample].input;
				input_nodes[param.num_input] = 1.0; //閾値

				//各層の素子の状態を更新
				updateNodesStateForward();

				//誤差の評価
				for( int j = 0; j < param.num_output; j++ ){
					max_error = std::max( max_error, pow( ( target[isample].output[j] -output_nodes[j] ), 2 ) );
					//output log
					sprintf( buf, "訓練データNO.%d, 誤差 = %G\n", isample+1, pow( ( target[isample].output[j] -output_nodes[j] ), 2 ));
					ofs << buf;

				}

				//逆方向に動作させる
				updateNodesStateBackword( isample );
				calcPartial();
			}
			optimizeWeightByBatch();
			//誤差が小さくなったらループを抜ける。
			sprintf( buf, "%G\n", max_error);
			max_error_log << buf;
			printf("%5d, Max error = %G\r", ilearn, max_error);
			if( max_error < param.threshold_error ){
				break;
			} else {
				//Not Reachec.
			}
		}
		printf( "学習回数:%d, 誤差:%G\n",ilearn, max_error );
		param.is_trained = true;
	}
}

void BackPropagation::calcPartial()
{
	//double tmp = 0;
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			// tmp = w1_partials[i][j];
			// w1_partials[i][j] += (- param.learning_coefficient *input_nodes[i] * h_back[j]) + param.interia_coefficient * w1_inertia_term[i][j];
			// w1_inertia_term[i][j] = tmp;
			 w1_partials[i][j] += (- param.learning_coefficient *input_nodes[i] * h_back[j]);
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			// tmp = w2_partials[i][j];
			// w2_partials[i][j] += (- param.learning_coefficient *hidden_nodes[i] * y_back[j]) + param.interia_coefficient * w2_inertia_term[i][j];
			// w2_inertia_term[i][j] = tmp;
			w2_partials[i][j] += (- param.learning_coefficient *hidden_nodes[i] * y_back[j]);
		}
	}

}

void BackPropagation::resetPartial()
{
	unsigned int size;
	for( unsigned int i = 0; i < w1_partials.size(); i++ ){
		size = w1_partials.size();
		w1_partials[i].assign( size, 0.0 );
	}

	for( unsigned int i = 0; i < w2_partials.size(); i++ ){
		size = w2_partials.size();
		w2_partials[i].assign( size, 0.0 );
	}
}

//結線重みの修正
void BackPropagation::optimizeWeightByBatch()
{
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			w1[i][j] +=  w1_partials[i][j];
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			w2[i][j] += w2_partials[i][j];
		}
	}
	resetPartial();
}

//online learning mode
void BackPropagation::learnOnline()
{
	int ilearn, isample;

	initLearn();
	char buf[1024];
	std::ofstream ofs("train_online.log");
	std::ofstream error_log("max_error_online.log");

	double error = 0, max_error = 0;
	for( ilearn = 0; ilearn < param.num_learn; ilearn++ ){
		max_error = 0;
		for( isample = 0; isample < param.num_sample; isample++ ){
			//順方向の動作
			//入力値を訓練データからxにつっこむ
			input_nodes = target[isample].input;
			input_nodes[param.num_input] = 1.0; //閾値

			//入力から出力の値を計算
			updateNodesStateForward();

			//誤差の評価
			error = checkError( target[isample] );
			if( error > max_error ){
				max_error = error;
			} else {
				//Not Reachec.
			}

			//logの生成
			sprintf( buf, "学習回数 = %d, 訓練データNO.%d, 誤差 = %G\n", ilearn, isample+1, error);
			ofs << buf;

			//逆方向の動作
			updateNodesStateBackword( isample );
			//重みの修正
			optimizeWeightOnline();
		}
		error_log << max_error << std::endl;

		//誤差が小さくなったらループを抜ける。
		if( max_error < param.threshold_error ){
			break;
		} else {
			//Not Reached.
		}
	}
	printf( "学習回数:%d, 誤差:%G\n",ilearn, max_error );
	param.is_trained = true;
}


//結線重みの修正
void BackPropagation::optimizeWeightOnline()
{
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			w1[i][j] -= param.learning_coefficient * input_nodes[i] * h_back[j];
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			w2[i][j] -= param.learning_coefficient * hidden_nodes[i] * y_back[j];
		}
	}
}

//誤差のチェック
double BackPropagation::checkError( const TrainingData& target )
{
	double error = 0;
	for( int j = 0; j < param.num_output; j++ ){
		error += pow( (target.output[j] - output_nodes[j]), 2 );
	}
	error /=  static_cast<double>( param.num_output );
	return error;
}

//逆方向の動作
void BackPropagation::updateNodesStateBackword( const int isample )
{
	TrainingData target = this->target[isample];
	//出力層素子
	for( int j = 0; j < param.num_output; j++ ){
		y_back[j] = (output_nodes[j] - target.output[j]) * (1.0 - output_nodes[j]) * output_nodes[j] * param.s_gain;
	}

	//隠れ層素子の逆伝搬時の動作
	for( int i = 0; i < param.num_hidden; i++ ){
		double input_to_node = 0;
		for( int j = 0; j < param.num_output; j++ ){
			input_to_node += w2[i][j] * y_back[j];
		}
		h_back[i] = input_to_node * (1.0 - hidden_nodes[i]) * hidden_nodes[i] * param.s_gain;
	}
}