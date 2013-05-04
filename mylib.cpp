#include<cmath>
#include<cstdlib>
#include<cstdio>
#include"mylib.h"
#include<iostream>
//シグモイド関数
double sigmoid( const double s , const double gain){
	return 1 - (1 / ((double)1 + exp(gain * s)));
}

//-1から1の範囲の乱数を返す。
double d_rand(){
	double ret = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	return (rand()%2 == 0) ? ret : -ret;
}

//パラメータの設定
void config(params* param){
	printf("隠れ層素子数(現在%d) > ",param->num_hidden);
	param->num_hidden = input_key<int>(1,100);
	printf("シグモイド関数ゲイン(現在%lf)", param->s_gain);
	param->s_gain = input_key<double>(-10.0, 10.0);
	printf("学習重みε(現在%lf)",param->epsilon);
	param->epsilon = input_key<double>(0,10.0);
	printf("許容誤差(現在%lf)",param->threshold_error);
	param->threshold_error = input_key<double>(0.00001,100);
}
