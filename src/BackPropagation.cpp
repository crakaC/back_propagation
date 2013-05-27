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
		num_hidden_layer = 2;
		sigmoid_gain = 1.0;
		learning_coefficient = 0.07;
		interia_coefficient = 0.05;
		threshold_error = 0.00001;
		is_empty = true;
		is_trained = false;
}

Node::Node(){
}

void Node::updateStateForward( const double gain )
{
	double total_input = 0;
	for(auto it = weight_from.begin(); it != weight_from.end(); it++){
		total_input += (it->second) * (it->first->value);
	}
	value = sigmoid( total_input, gain );
}

void Node::updateStateBackword( const double gain  )
{
	//隠れ層素子の逆伝搬時の動作
	double total_input = 0;
	for(auto it = weight_to.begin(); it != weight_to.end(); it++){
		total_input += it->second * it->first->back_value;		
	}
	back_value = total_input * (1.0 - value) * value * gain;
}

//シグモイド関数
double Node::sigmoid( const double inputs, const double gain ){
	return  1 / ( 1.0 + exp( gain * (-inputs) ) );
}

BackPropagation::BackPropagation(){
}

//-1から1の範囲の乱数を返す。
double BackPropagation::dRand(){
	double ret = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
	return ( rand()%2 == 0 ) ? ret : -ret;
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




//各素子を初期化
void BackPropagation::initialize()
{
	int last_layer = param.num_hidden_layer - 1;
	//init nodes
	input_nodes.assign( param.num_input + 1, Node() );
	hidden_nodes.assign( param.num_hidden_layer, std::vector< Node >( param.num_hidden + 1 ) );
	output_nodes.assign( param.num_output, Node() );

	//generate graph and init weight
	for( int i = 0; i < param.num_input + 1; i++ ) {
		for( int j = 0; j < (int) hidden_nodes[0].size(); j++ ) {
			//between input layer and hidden layer
			input_nodes[i].weight_to[ &hidden_nodes[0][j] ] = dRand();
			hidden_nodes[0][j].weight_from[&input_nodes[i]] = dRand();
			printf("input node[%d] <-> hidden_nodes[0]%d\r", i, j);
			if( i == param.num_input){
				hidden_nodes[0][j].weight_from[&input_nodes[i]] = 1.0;//ノードの閾値
			}
		}
	}
	//hidden_nodes.size() : num of hidden layers
	//hidden_nodess[l].size() : num of nodes of layer l.(include threshold)
	for( int l = 0, size1 = (int) hidden_nodes.size() - 1; l < size1; l++ ) {
		for( int i = 0, size2 = (int) hidden_nodes[l].size(); i < size2; i++) {
			for( int j = 0, size3 = (int) hidden_nodes[l+1].size() - 1; j < size3; j++) {
				hidden_nodes[l][i].weight_to[ &hidden_nodes[l + 1][j] ] = dRand();
				hidden_nodes[l + 1][j].weight_from[ &hidden_nodes[l][i] ] = dRand();
				if( i == size2 - 1 ) {
					hidden_nodes[l + 1][j].weight_from[ &hidden_nodes[l][i] ] = 1.0;
				}
			}
		}
	}

	for( int i = 0, size = (int) hidden_nodes[last_layer].size(); i < size; i++ ) {
		for( int j = 0; j < param.num_output; j++ ) {
			//between input layer and hidden layer
			hidden_nodes[last_layer][i].weight_to[ &output_nodes[j] ] = dRand();
			output_nodes[j].weight_from[ &hidden_nodes[last_layer][i] ] = dRand();
			if( i == (int)hidden_nodes[last_layer].size() - 1 ) {
				output_nodes[j].weight_from[ &hidden_nodes[last_layer][i] ] = 1.0;//ノードの閾値
			}
		}
	}

	//設定を記憶
	param_bk = param;
	printf( "hidden_nodes = %d, gain = %lf, learning_coefficient = %lf, threshold_error = %G\n", param.num_hidden, param.sigmoid_gain, param.learning_coefficient, param.threshold_error );
}


//与えられた入力データを用いて出力データを返す．
std::vector< double > BackPropagation::output( const std::vector< double >& input )
{
	std::vector< double > output( param_bk.num_output );

	for( int i = 0; i < param_bk.num_input; i++) {
		input_nodes[i].value = input[i];
	}

	updateNodesStateForward();

	for( int i = 0; i < param_bk.num_output; i++ ){
		output[i] = output_nodes[i].value;
	}

	return output;
}

//メンバのxから,yを更新する。
void BackPropagation::updateNodesStateForward()
{
	//update hidden layer nodes.
	for( int l = 0; l < param.num_hidden_layer; l++ ) {
		for( int i = 0; i < param.num_hidden; i++ ) {
			hidden_nodes[l][i].updateStateForward( param.sigmoid_gain );
		}
	}

	//update output layer nodes
	for( int i = 0; i < param.num_output; i++ ) {
		output_nodes[i].updateStateForward( param.sigmoid_gain );
	}
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
		initialize();//check training data and initialize Nodes and Weight
		sprintf( buf, "hidden_nodes = %d, gain = %lf, learning_coefficient = %lf, threshold_error = %G\n", param.num_hidden, param.sigmoid_gain, param.learning_coefficient, param.threshold_error );
		ofs << buf;
		for( ilearn = 0; ilearn < param.num_learn; ilearn++ ){
			max_error = 0;
			sprintf( buf, "\n学習回数 = %d\n", ilearn );
			ofs << buf;
			for( isample = 0 ; isample < param.num_sample; isample++ ){
				//順方向の動作
				//入力値を訓練データからx(入力層素子)につっこむ
				for( int i = 0; i < param.num_input; i++) {
					input_nodes[i].value = target[isample].input[i];
				}
				//各層の素子の状態を更新
				updateNodesStateForward();

				//誤差の評価
				for( int j = 0; j < param.num_output; j++ ){
					max_error = std::max( max_error, pow( ( target[isample].output[j] -output_nodes[j].value ), 2 ) );
					//output log
					sprintf( buf, "訓練データNO.%d, 誤差 = %G (目標:%f 出力:%f)\n", isample+1, pow( ( target[isample].output[j] -output_nodes[j].value ), 2 ), target[isample].output[j], output_nodes[j].value);
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
	/*
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
	*/

}

void BackPropagation::resetPartial()
{
	/*
	unsigned int size;
	for( unsigned int i = 0; i < w1_partials.size(); i++ ){
		size = w1_partials.size();
		w1_partials[i].assign( size, 0.0 );
	}

	for( unsigned int i = 0; i < w2_partials.size(); i++ ){
		size = w2_partials.size();
		w2_partials[i].assign( size, 0.0 );
	}
	*/
}

//結線重みの修正
void BackPropagation::optimizeWeightByBatch()
{
	/*
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
	*/
}

//online learning mode
void BackPropagation::learnOnline()
{
	int ilearn, isample;

	char buf[1024];
	std::ofstream ofs("train_online.log");
	std::ofstream error_log("max_error_online.log");
	double error = 0, max_error = 0;

	if( param.is_empty ){
		printf("input training data first.\n");
	} else {
		initialize();

		for( ilearn = 0; ilearn < param.num_learn; ilearn++ ){
			max_error = 0;
			for( isample = 0; isample < param.num_sample; isample++ ){
				//順方向の動作
				//入力値を訓練データからxにつっこむ
				for( int i = 0; i < param.num_input; i++) {
					input_nodes[i].value = target[isample].input[i];
				}
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
				for( int j = 0; j < param.num_output; j++ ){
					//output log
					sprintf( buf, "訓練データNO.%d, 誤差 = %G (目標:%f 出力:%f)\n", isample+1, error, target[isample].output[j], output_nodes[j].value);
					ofs << buf;

				}
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
}


//結線重みの修正
void BackPropagation::optimizeWeightOnline()
{
	int last_layer = param.num_hidden_layer - 1;
	//between input layer and hidden layer
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0, size = (int)hidden_nodes[0].size() - 1; j < size; j++ ){
			input_nodes[i].weight_to[ &hidden_nodes[0][j] ] -= param.learning_coefficient * input_nodes[i].value * hidden_nodes[0][j].back_value;
			hidden_nodes[0][j].weight_from[ &input_nodes[i] ] = input_nodes[i].weight_to[ &hidden_nodes[0][j] ];
		}
	}

	//hidden layer - hidden layer
	for( int l = 0; l < param.num_hidden_layer - 1; l++ ){
		for( int i = 0, size = (int)hidden_nodes[l].size(); i < size; i++ ){
			for( int j = 0, size2 = (int)hidden_nodes[l + 1].size() - 1; j < size2; j++){
				hidden_nodes[l][i].weight_to[ &hidden_nodes[l + 1][j] ] -= param.learning_coefficient * hidden_nodes[l][i].value * hidden_nodes[l + 1][j].back_value;
				hidden_nodes[l + 1][j].weight_from[ &hidden_nodes[l][i] ] = hidden_nodes[l][i].weight_to[ &hidden_nodes[l + 1][j] ];
			}
		}
	}

	//hidden layer - output layer
	for( int i = 0, size = (int)hidden_nodes[last_layer].size(); i < size; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			hidden_nodes[last_layer][i].weight_to[ &output_nodes[j] ] -= param.learning_coefficient * hidden_nodes[last_layer][i].value * output_nodes[j].back_value;
			output_nodes[j].weight_from[ &hidden_nodes[last_layer][i] ] = hidden_nodes[last_layer][i].weight_to[ &output_nodes[j] ];
		}
	}
}

//誤差のチェック
double BackPropagation::checkError( const TrainingData& target )
{
	double error = 0;
	for( int j = 0; j < param.num_output; j++ ){
		error += pow( (target.output[j] - output_nodes[j].value), 2 );
	}
	error /=  static_cast<double>( param.num_output );
	return error;
}

//逆方向の動作
void BackPropagation::updateNodesStateBackword( const int isample )
{
	TrainingData target = this->target[isample];
	//出力層素子
	for( int i = 0; i < param.num_output; i++ ){
		output_nodes[i].back_value = (output_nodes[i].value - target.output[i]) * (1.0 - output_nodes[i].value) * output_nodes[i].value * param.sigmoid_gain;
	}

	//hidden layer
	for( int l = param.num_hidden_layer - 1; l >= 0; l-- ){
		for( int i = 0, size = hidden_nodes[l].size(); i < size; i++ ){
			hidden_nodes[l][i].updateStateBackword( param.sigmoid_gain );
		}
	}
}