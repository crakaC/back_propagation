#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<fstream>
#include"Net.hpp"

Net::Net(){
}

void Net::setTrainingData( const std::string filename = "training2.dat" )
{
	using namespace std;
	ifstream ifs;
	ifs.open( filename.c_str() );
	//ファイルを開けなかったら戻る。
	if( !ifs.is_open() ){
		cout << "cannot open " << filename << endl;
		return;
	}

	//データからパラメータを読み込む
	ifs >> param.num_sample >> param.num_input >> param.num_output;
	if( param.num_sample < 1 || param.num_input < 1 || param.num_output < 1 ){
		cout<< "file format is wrong." <<endl;
		return;
	}
	param.is_empty = false;
	param.is_trained = false;

	target.resize( param.num_sample );
	//訓練データをtargetに格納
	for( int isample = 0; isample < param.num_sample ; isample++ ){
		target[isample] =  TrainingData( param.num_input, param.num_output );
		for( int i = 0; i < param.num_input; i++ ){
			ifs >> target[isample].input[i];
		}
		for( int i = 0; i < param.num_output; i++ ){
			ifs >> target[isample].output[i];
		}
	}

	//訓練データを表示
	printf( "***** input training data from %s *****\n", filename.c_str() );
	for( int isample = 0; isample < param.num_sample; isample++ ){
		printf( "訓練データ No.%d\n", isample + 1 );
		printf( "入力：" );
		int size = (int)target[isample].input.size();
		for( int i = 0; i < size; i++ ){
			cout << target[isample].input[i];
			if( i < size - 1 ){
				cout << ' ';
			}
		}
		printf("\n出力：");
		size = (int)target[isample].output.size();
		for(int i = 0; i < size; i++){
			cout << target[isample].output[i];
			if( i < size - 1 ){
				cout << ' ';
			}
		}
		cout << endl;
	}
	printf("*****************************************\n");
}


//シグモイド関数
double Net::sigmoid( const double s ){
	return  1 / ( (double)1 + exp( param.s_gain * (-s) ) );
}

//-1から1の範囲の乱数を返す。
double Net::dRand(){
	double ret = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
	return ( rand()%2 == 0 ) ? ret : -ret;
}

//各素子を初期化
void Net::initNode()
{
	//学習用
	x.resize( param.num_input + 1 );
	h.resize( param.num_hidden + 1 );
	y.resize( param.num_output );
	h_back.resize( param.num_hidden );
	y_back.resize( param.num_output );

	//設定を記憶
	param_bk = param;
}

//結線の重みを初期化
void Net::initWight(){

	w1.clear();
	w1.resize( param.num_input + 1, std::vector< double > ( param.num_hidden ) );
	w2.clear();
	w2.resize( param.num_hidden + 1, std::vector< double > ( param.num_output ) );

	w1_partials.clear();
	w1_partials.assign( param.num_input + 1, std::vector< double > ( param.num_hidden, 0.0 ) );
	w2_partials.clear();
	w2_partials.assign( param.num_hidden + 1, std::vector< double > ( param.num_output, 0.0 ) );

	w1_inertia_term.clear();
	w1_inertia_term.assign( param.num_input + 1, std::vector< double > ( param.num_hidden, 0.0 ) );
	w2_inertia_term.clear();
	w2_inertia_term.assign( param.num_hidden + 1, std::vector< double > ( param.num_output, 0.0 ) );

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
std::vector< double > Net::output( const std::vector< double >& input )
{
	std::vector< double > output( param_bk.num_output );
	double net_input = 0;
	//隠れ素子の値を計算
	for( int j = 0; j < param_bk.num_hidden ; j++ ){
		net_input = 0;	
		for( int i = 0; i < param_bk.num_input + 1; i++ ){
			net_input += w1[i][j] * input[i];
		}
		h[j] = sigmoid( net_input );
	}

	//出力値の計算
	for( int j = 0; j < param_bk.num_output; j++ ){
		net_input = 0;
		for( int i = 0; i < param_bk.num_hidden + 1; i++ ){
			net_input += w2[i][j] * h[i];
		}
		output[j] = sigmoid( net_input );
	}

	return output;
}

//メンバのxから,yを更新する。
void Net::updateNodesState()
{
	double net_input;
	//隠れ素子値の計算
	for( int j = 0; j < param.num_hidden; j++ ){
		net_input = 0;
		for( int i = 0; i < param.num_input + 1; i++ ){
			net_input += w1[i][j] * x[i];
		}
		h[j] = sigmoid( net_input );
	}

	//出力値の計算
	for( int j = 0; j < param.num_output; j++ ){
		net_input = 0;
		for( int i = 0; i < param.num_hidden + 1; i++ ){
			net_input += w2[i][j] * h[i];
		}
		y[j] = sigmoid( net_input );
	}
}

void Net::initLearn()
{
	if( param.is_empty ){
		printf( "先に訓練データを入力してください\n" );
		return;
	}
	printf( "hidden_nodes = %d, gain = %lf, learning_coefficient = %lf, threshold_error = %G\n", param.num_hidden, param.s_gain, param.learning_coefficient, param.threshold_error );
	initNode();
	initWight();
	return;
}

void Net::learnBatch()
{
	initLearn();

	char buf[1024];
	std::ofstream ofs("train_batch.log");

	int ilearn;
	double max_error = 0;
	for( ilearn = 0; ilearn < param.num_learn; ilearn++ ){
		max_error = 0;
		int isample = 0;
		for( isample = 0 ; isample < param.num_sample; isample++ ){
			//順方向の動作
			//入力値を訓練データからxにつっこむ
			x = target[isample].input;
			x[param.num_input] = 1.0; //閾値

			//入力から出力の値を計算
			updateNodesState();

			//誤差の評価
			for( int j = 0; j < param.num_output; j++ ){
				max_error = std::max( max_error, pow( ( target[isample].output[j] - y[j] ), 2 ) );
				//logの生成
				sprintf( buf, "学習回数 = %d, 訓練データNO.%d, 誤差 = %G\n", ilearn, isample+1, pow( ( target[isample].output[j] - y[j] ), 2 ));
				ofs << buf;

			}
			reverse( isample );
			calcPartial();
		}
		fixWeightByBatch();
		//誤差が小さくなったらループを抜ける。
		if( max_error < param.threshold_error ){
			break;
		}
	}
	printf( "学習回数:%d, 誤差:%G\n",ilearn, max_error );
	param.is_trained = true;
}


void Net::calcPartial()
{
	//double tmp = 0;
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			// tmp = w1_partials[i][j];
			// w1_partials[i][j] += (- param.learning_coefficient * x[i] * h_back[j]) + param.interia_coefficient * w1_inertia_term[i][j];
			// w1_inertia_term[i][j] = tmp;
			 w1_partials[i][j] += (- param.learning_coefficient * x[i] * h_back[j]);
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			// tmp = w2_partials[i][j];
			// w2_partials[i][j] += (- param.learning_coefficient * h[i] * y_back[j]) + param.interia_coefficient * w2_inertia_term[i][j];
			// w2_inertia_term[i][j] = tmp;
			w2_partials[i][j] += (- param.learning_coefficient * h[i] * y_back[j]);
		}
	}

}

void Net::resetPartial()
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
void Net::fixWeightByBatch()
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


void Net::learnOnline()
{
	initLearn();
	//char buf[1024];
	//std::ofstream ofs("train.log");
	int ilearn;
	double error = 0, max_error = 0;
	for( ilearn = 0; ilearn < param.num_learn; ilearn++ ){
		max_error = 0;
		for( int isample = 0; isample < param.num_sample; isample++ ){
			//順方向の動作
			//入力値を訓練データからxにつっこむ
			x = target[isample].input;
			x[param.num_input] = 1.0; //閾値

			//入力から出力の値を計算
			updateNodesState();

			//誤差の評価
			error = checkError( target[isample] );
			if( error > max_error ){
				max_error = error;
			}

			//logの生成
			//sprintf( buf, "学習回数 = %d, 訓練データNO.%d, 誤差 = %G\n", ilearn, isample+1, error);
			//ofs << buf;

			//逆方向の動作
			reverse( isample );
			//重みの修正
			fixWeightOnline();
		}

		//誤差が小さくなったらループを抜ける。
		if( max_error < param.threshold_error ){
			break;
		}
	}
	printf( "学習回数:%d, 誤差:%G\n",ilearn, max_error );
	param.is_trained = true;
}


//結線重みの修正
void Net::fixWeightOnline()
{
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			w1[i][j] -= param.learning_coefficient * x[i] * h_back[j];
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			w2[i][j] -= param.learning_coefficient * h[i] * y_back[j];
		}
	}
}

//誤差のチェック
double Net::checkError( const TrainingData& target )
{
	double error = 0;
	for( int j = 0; j < param.num_output; j++ ){
		error += pow( (target.output[j] - y[j]), 2 );
	}
	error /=  static_cast<double>( param.num_output );
	return error;
}

//逆方向の動作
void Net::reverse( const int isample )
{
	TrainingData target = this->target[isample];
	//出力層素子
	for( int j = 0; j < param.num_output; j++ ){
		y_back[j] = (y[j] - target.output[j]) * (1.0 - y[j]) * y[j] * param.s_gain;
	}

	//隠れ層素子の逆伝搬時の動作
	for( int i = 0; i < param.num_hidden; i++ ){
		double net_input = 0;
		for( int j = 0; j < param.num_output; j++ ){
			net_input += w2[i][j] * y_back[j];
		}
		h_back[i] = net_input * (1.0 - h[i]) * h[i] * param.s_gain;
	}
}
