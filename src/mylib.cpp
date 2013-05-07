#include<cmath>
#include<cstdlib>
#include<cstdio>
#include<climits>
#include<string>
#include<iostream>
#include<fstream>
#include"mylib.h"

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

//コマンドライン用メッセージその１
std::vector< std::string > gen_msg()
{
	std::vector< std::string > msg;
	msg.push_back("訓練データ入力");
	msg.push_back("学習");
	msg.push_back("試してみる");
	msg.push_back("一括パラメータ調整");
	msg.push_back("パラメータ調整");
	msg.push_back("訓練データ作成");
	msg.push_back("終了");

	return msg;
}

//コマンドライン用メッセージその２
std::vector< std::string > gen_msg2()
{
	std::vector< std::string > msg;
	msg.push_back("中間層素子数設定");
	msg.push_back("学習回数設定");
	msg.push_back("許容誤差設定");
	msg.push_back("ゲイン設定");
	msg.push_back("学習重み設定");
	msg.push_back("戻る");

	return msg;
}

void show_msg(const std::vector< std::string > msg)
{
	int i;
	for( i = 0; i < (int)msg.size() - 1; i++ ){
		std::cout << i+1 << ':' << msg[i] << std::endl;
	}
	std::cout << "0:" << msg[i] << std::endl;
}
