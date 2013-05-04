#ifndef MYLIB_H_20130504
#define MYLIB_H_20130504

#include<iostream>
#include<climits>
#include"struct.h"

//シグモイド関数
//引数 s:細胞内電位, gain:ゲイン
//返り値 0~1
double sigmoid( const double s , const double gain );

//擬似乱数
//返り値 -1~1
double d_rand();

//キー入力を指定した値の範囲に制限。
template<class T> T input_key( T min, T max ){
	using namespace std;

	T value;
	cin >> value;
	while( !cin || ( value < min || max < value ) ){
		cout << min << "から" << max << "の間で入力することを強いられているんだ…！" << endl;
		cin.clear();
		cin.ignore( INT_MAX, '\n' );
		cout << '>' << flush;
		cin >> value;
	}
	return value;
}

#endif
