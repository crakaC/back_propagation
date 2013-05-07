#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<fstream>
#include"Net.h"

Net::Net(){
}

void Net::set_training_data( const std::string filename = "training2.dat" )
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

	//訓練データをtargetに格納
	for( int isample = 0; isample < param.num_sample ; isample++ ){
		target.push_back( TrainingData( param.num_input, param.num_output ) );
		for( int i = 0; i < param.num_input; i++ ){
			ifs >> target[isample].input[i];
		}
		for( int i = 0; i < param.num_output; i++ ){
			ifs >> target[isample].output[i];
		}
	}

	//訓練データを表示
	printf( "input training data from %s\n", filename.c_str() );
	for( int isample = 0; isample < param.num_sample; isample++ ){
		cout << "訓練データ No." << isample+1 << endl;
		cout << "入力：";
		int size = (int)target[isample].input.size();
		for( int i = 0; i < size; i++ ){
			cout << target[isample].input[i];
			if( i < size - 1 ){
				cout << ' ';
			}
		}
		cout << endl << "出力：";
		size = (int)target[isample].output.size();
		for(int i = 0; i < size; i++){
			cout << target[isample].output[i];
			if( i < size - 1 ){
				cout << ' ';
			}
		}
		cout << endl;
	}
}


void Net::init_weight(){
	//結線の重みを初期化
	srand( time( NULL ) );
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			w1[ Pair(i, j) ] = d_rand();
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			w2[ Pair(i, j) ] = d_rand();
		}
	}
}

//シグモイド関数
double Net::sigmoid( const double s ){
	return  1 / ( (double)1 + exp( param.s_gain * (-s) ) );
}

//-1から1の範囲の乱数を返す。
double Net::d_rand(){
	double ret = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
	return ( rand()%2 == 0 ) ? ret : -ret;
}

//各素子を初期化
void Net::init_node()
{
	x.assign( param.num_input + 1, (double)0 );
	h.assign( param.num_hidden + 1, (double)0 );
	y.assign( param.num_output, (double)0 );
	h_back.assign( param.num_hidden, (double)0 );
	y_back.assign( param.num_output, (double)0 );

	//設定を記憶
	param_bk = param;
}

//与えられた入力データを用いて出力データを返す．
std::vector< double > Net::output( const std::vector< double >& input )
{
	std::vector< double > output( param_bk.num_output );

	//隠れ素子の値を計算
	for( int j = 0; j < param_bk.num_hidden ; j++ ){
		double net_input = 0;
		for( int i = 0; i < param_bk.num_input + 1; i++ ){
			net_input += w1[ Pair(i, j) ] * input[i];
		}
		h[j] = sigmoid( net_input );
	}

	//出力値の計算
	for( int j = 0; j < param_bk.num_output; j++ ){
		double net_input = 0;
		for( int i = 0; i < param_bk.num_hidden + 1; i++ ){
			net_input += w2[ Pair(i, j) ] * h[i];
		}
		output[j] = sigmoid( net_input );
	}

	return output;
}

//メンバのxから,yを更新する。
void Net::update_y()
{
	double net_input;
	//隠れ素子値の計算
	for( int j = 0; j < param.num_hidden; j++ ){
		net_input = 0;
		for( int i = 0; i < param.num_input + 1; i++ ){
			net_input += w1[ Pair(i, j) ] * x[i];
		}
		h[j] = sigmoid( net_input );
	}

	//出力値の計算
	for( int j = 0; j < param.num_output; j++ ){
		net_input = 0;
		for( int i = 0; i < param.num_hidden + 1; i++ ){
			net_input += w2[ Pair(i, j) ] * h[i];
		}
		y[j] = sigmoid( net_input );
	}
}

void Net::learn()
{
	if( param.is_empty ){
		printf( "先に訓練データを入力してください\n" );
		return;
	}
	printf( "gain = %lf, epsilon = %lf, threshold_error = %lf\n", param.s_gain, param.epsilon, param.threshold_error );
	init_node();
	init_weight();

	char buf[1024];
	std::ofstream ofs("train.log");

	int ilearn;
	double error = 0, max_error = 0;
	for( ilearn = 0; ilearn < param.num_learn; ilearn++ ){
		max_error = 0;
		for( int isample = 0; isample < param.num_sample; isample++ ){
			//順方向の動作
			//入力値を訓練データからxにつっこむ
			for( int i = 0; i < param.num_input; i++ ){
				x[i] = target[isample].input[i];
			}
			x[param.num_input] = 1.0; //閾値

			//入力から出力の値を計算
			update_y();

			//誤差の評価
			error = check_error( target[isample] );
			if( error > max_error ){
				max_error = error;
			}

			sprintf( buf, "学習回数 = %d, 訓練データNO.%d, 誤差 = %G\n", ilearn, isample+1, error);
			ofs << buf;

			//逆方向の動作
			//出力層素子
			reverse( target[isample] );
			//重みの修正
			fix_weight();
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
void Net::fix_weight()
{
	for( int i = 0; i < param.num_input + 1; i++ ){
		for( int j = 0; j < param.num_hidden; j++ ){
			w1[Pair(i, j)] -= param.epsilon * x[i] * h_back[j];
		}
	}
	for( int i = 0; i < param.num_hidden + 1; i++ ){
		for( int j = 0; j < param.num_output; j++ ){
			w2[Pair(i, j)] -= param.epsilon * h[i] * y_back[j];
		}
	}
}

//誤差のチェック
double Net::check_error( const TrainingData& target )
{
	double error = 0;
	for( int j = 0; j < param.num_output; j++ ){
		error += pow( (target.output[j] - y[j]), 2 );
	}
	error /=  static_cast<double>( param.num_output );
	return error;
}

//逆方向の動作
void Net::reverse( const TrainingData& target )
{
	//出力層素子
	for( int j = 0; j < param.num_output; j++ ){
		y_back[j] = (y[j] - target.output[j]) * (1.0 - y[j]) * y[j] * param.s_gain;
	}

	//隠れ層素子の逆伝搬時の動作
	for( int i = 0; i < param.num_hidden; i++ ){
		double net_input = 0;
		for( int j = 0; j < param.num_output; j++ ){
			net_input += w2[ Pair(i,j)] * y_back[j];
		}
		h_back[i] = net_input * (1.0 - h[i]) * h[i] * param.s_gain;
	}
}


