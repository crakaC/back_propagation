#include<cstdio>
#include"struct.h"
#include"mylib.h"

Params::Params()
{
		num_input = num_hidden = num_output = 0;
		num_learn = 10000;
		num_hidden = 10;
		s_gain = 1.0;
		epsilon = 0.05;
		threshold_error = 0.01;
		is_empty = true;
		is_trained = false;
}

Params::Params( const Params& p ){
	fprintf(stderr,"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
		   "コピーコンストラクタktkr\n"
		   "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
}

void Params::set_all()
{
	set_learn_num();
	set_hidden_num();
	set_s_gain();
	set_epsilon();
	set_threshold_error();
}

void Params::set_learn_num()
{
	printf( "学習回数(現在%d) > ", num_learn );
	num_learn = input_key<int>( 1, INT_MAX - 1 );
}

void Params::set_hidden_num()
{
	printf( "隠れ層素子数(現在%d) > ", num_hidden );
	num_hidden = input_key<int>( 1, 100 );
}
void Params::set_s_gain()
{
	printf( "シグモイド関数ゲイン(現在%lf)", s_gain );
	s_gain = input_key<double>( -10.0, 10.0 );
}
void Params::set_epsilon()
{
	printf( "学習重みε(現在%lf)", epsilon );
	epsilon = input_key<double>( 0, 10.0 );
}

void Params::set_threshold_error()
{
	printf( "許容誤差(現在%lf)", threshold_error );
	threshold_error = input_key<double>( 0.00001, 100 );
}
