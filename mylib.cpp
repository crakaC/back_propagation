#include<cmath>
#include<cstdlib>
#include"mylib.h"

//シグモイド関数
double sigmoid( const double s , const double gain){
	return 1 - (1 / ((double)1 + exp(gain * s)));
}

//-1から1の範囲の乱数を返す。
double d_rand(){
	double ret = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	return (rand()%2 == 0) ? ret : -ret;
}
