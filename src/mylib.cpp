#include<cmath>
#include<cstdlib>
#include<cstdio>
#include<climits>
#include<string>
#include<iostream>
#include<fstream>
#include"mylib.h"
//シグモイド関数
double sigmoid( const double s , const double gain ){
	return  1 / ( (double)1 + exp( gain * (-s) ) );
}

//-1から1の範囲の乱数を返す。
double d_rand(){
	double ret = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
	return ( rand()%2 == 0 ) ? ret : -ret;
}

//訓練データを作成する
void create_training_data()
{
	using namespace std;
	string filename;
	ofstream ofs;
	int sample_num, input_num, output_num;
	cout << "output file name > " << flush;
	cin >> filename;

	//ファイルをオープン
	ofs.open( filename.c_str() );
	if( !ofs.is_open() ){
		cout <<"cannot create " << filename << '.' << endl;
		return;
	}

	//訓練データ数、入力数、出力数を入力
	cout << "data number " << flush;
	sample_num = input_key< int >( 1, INT_MAX - 1 );
	cout << "input number " << flush;
	input_num = input_key< int >( 1, INT_MAX - 1 );
	cout << "output number " << flush;
	output_num = input_key< int >( 1, INT_MAX - 1 );

	//流しこむ
	ofs << sample_num << ' ' << input_num << ' ' << output_num << endl;

	//データを作成
	for(int i = 0; i < sample_num; i++){
		double* p = new double[ input_num  ];
		for(int j = 0; j < input_num; j++){
			printf("[%d/%d]input%d > ", i + 1, sample_num, j + 1 );
			cin >> p[j];
		}
		for(int j = 0; j < input_num; j++){
			ofs << p[j] <<' ';
		}
		delete[] p;

		p = new double[ output_num ];
		for(int j = 0; j < output_num; j++){
			printf("[%d/%d]output%d > ", i + 1, sample_num, j + 1 );
			cin >> p[j];
		}
		for(int j = 0; j < input_num; j++){
			if( j < input_num - 1 ){
				ofs << p[j] <<' ';
			}
			else{
				ofs << p[j] << endl;
			}
		}
		delete[] p;
	}
	printf( "データを作成しました(%s)\n", filename.c_str() );
}
