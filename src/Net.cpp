#include<cstdlib>
#include<cstdio>
#include<cmath>
#include"Net.h"
#include"mylib.h"

Net::Net( Params* p ){
	this->param = p;
}

void Net::init_weight(){
	//結線の重みを初期化
	srand( time( NULL ) );
	for( int i = 0; i < this->param->num_input + 1; i++ ){
		for( int j = 0; j < this->param->num_hidden; j++ ){
			w1[ Pair(i, j) ] = d_rand();
		}
	}
	for( int i = 0; i < this->param->num_hidden + 1; i++ ){
		for( int j = 0; j < this->param->num_output; j++ ){
			w2[ Pair(i, j) ] = d_rand();
		}
	}
}

void Net::init_node()
{
	//各素子を初期化
	this->x.assign( param->num_input + 1, (double)0 );
	this->h.assign( param->num_hidden + 1, (double)0 );
	this->y.assign( param->num_output, (double)0 );
	this->h_back.assign( param->num_hidden, (double)0 );
	this->y_back.assign( param->num_output, (double)0 );

	//設定を記憶 Paramにポインタはないからこれで大丈夫なはず。
	param_bk = *param;
}

std::vector< double > Net::output( const std::vector< double >& input )
{
	std::vector< double > output( param_bk.num_output );

	//状態の表示
	printf( "***********Configuration***********\n"
			"Hidden Nodes = %d, gain = %lf\n"
			"***********************************\n", param_bk.num_hidden, param_bk.s_gain );
	//隠れ素子値の計算
	for( int j = 0; j < param_bk.num_hidden ; j++ ){
		double net_input = 0;
		for( int i = 0; i < param_bk.num_input + 1; i++ ){
			net_input += this->w1[ Pair(i, j) ] * input[i];
		}
		this->h[j] = sigmoid( net_input, param_bk.s_gain );
	}

	//出力値の計算
	for( int j = 0; j < param_bk.num_output; j++ ){
		double net_input = 0;
		for( int i = 0; i < param_bk.num_hidden + 1; i++ ){
			net_input += this->w2[ Pair(i, j) ] * this->h[i];
		}
		output[j] = sigmoid( net_input, param_bk.s_gain );
	}

	return output;
}

//メンバのxから,yを更新する。
void Net::update_y()
{
	double net_input;
	//隠れ素子値の計算
	for( int j = 0; j < this->param->num_hidden; j++ ){
		net_input = 0;
		for( int i = 0; i < this->param->num_input + 1; i++ ){
			net_input += this->w1[ Pair(i, j) ] * this->x[i];
		}
		this->h[j] = sigmoid( net_input, this->param->s_gain );
	}

	//出力値の計算
	for( int j = 0; j < this->param->num_output; j++ ){
		net_input = 0;
		for( int i = 0; i < this->param->num_hidden + 1; i++ ){
			net_input += this->w2[ Pair(i, j) ] * this->h[i];
		}
		this->y[j] = sigmoid( net_input, this->param->s_gain );
	}
}

void Net::learn( const std::vector< TrainingData >& target )
{
	if( param->is_empty ){
		printf( "先に訓練データを入力してください\n" );
		return;
	}
	printf( "gain = %lf, epsilon = %lf, threshold_error = %lf\n", param->s_gain, param->epsilon, param->threshold_error );
	init_node();
	init_weight();

	int ilearn;
	double error, max_error;
	for( ilearn = 0; ilearn < param->num_learn; ilearn++ ){
		max_error = 0;
		for( int isample = 0; isample < param->num_sample; isample++ ){
			//順方向の動作
			//入力値を訓練データからxにつっこむ
			for( int i = 0; i < param->num_input; i++ ){
				this->x[i] = target[isample].input[i];
			}
			this->x[param->num_input] = 1.0; //閾値

			//入力から出力の値を計算
			update_y();

			//誤差の評価
			error = check_error( target[isample] );
			if( error > max_error ){
				max_error = error;
			}

			//printf("学習回数 = %d, 訓練データNO.%d, 誤差 = %lf\n", ilearn, isample+1, error);

			//逆方向の動作
			//出力層素子
			reverse( target[isample] );
			//重みの修正
			fix_weight();
		}

		//誤差が小さくなったらループを抜ける。
		if( max_error < param->threshold_error ){
			break;
		}
	}
	printf( "学習回数:%d, 誤差:%lf\n",ilearn, max_error );
	param->is_trained = true;
}

void Net::fix_weight()
{
	for( int i = 0; i < param->num_input + 1; i++ ){
		for( int j = 0; j < param->num_hidden; j++ ){
			w1[Pair(i, j)] -= param->epsilon * x[i] * h_back[j];
		}
	}
	for( int i = 0; i < param->num_hidden + 1; i++ ){
		for( int j = 0; j < param->num_output; j++ ){
			w2[Pair(i, j)] -= param->epsilon * h[i] * y_back[j];
		}
	}
}

double Net::check_error( const TrainingData& target )
{
	double error = 0;
	for( int j = 0; j < param->num_output; j++ ){
		error += pow( (target.output[j] - y[j]), 2 );
	}
	error = error / static_cast<double>( param->num_output );
	return error;
}

void Net::reverse( const TrainingData& target )
{
	//逆方向の動作
	//出力層素子
	for( int j = 0; j < param->num_output; j++ ){
		y_back[j] = (y[j] - target.output[j]) * (1.0 - y[j]) * y[j] * param->s_gain;
	}

	//隠れ層素子の逆伝搬時の動作
	for( int i = 0; i < param->num_hidden; i++ ){
		double net_input = 0;
		for( int j = 0; j < param->num_output; j++ ){
			net_input += w2[ Pair(i,j)] * y_back[j];
		}
		h_back[i] = net_input * (1.0 - h[i]) * h[i] * param->s_gain;
	}
}
