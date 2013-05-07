#ifndef MYLIB_H_20130504
#define MYLIB_H_20130504

#include<iostream>
#include<climits>
#include<vector>
#include<string>
#include"Net.hpp"

//キー入力を指定した値の範囲に制限。
template<class T> T input_key( T min, T max ){
	using namespace std;

	T value;
	cout << '>' <<flush;
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

char y_or_n();

//訓練データの作成
void create_training_data();

//自分で値を入力して実行
void test_bp( Net* net );
bool execute( Net* net );

//コマンドライン用メッセージ1
std::vector< std::string > gen_msg();

//コマンドライン用メッセージ2
std::vector< std::string > gen_msg2();

//コマンドライン用メッセージ表示
void show_msg(const std::vector< std::string > msg);

//ニューラルネット各種パラメータ設定
void set_all_params( Net* net );
void set_hidden_nodes_num( Net* net );
void set_learn_num( Net* net );
void set_threshold_error( Net* net );
void set_s_gain( Net* net );
void set_epsilon( Net* ney );

#endif
