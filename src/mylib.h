#ifndef MYLIB_H_20130504
#define MYLIB_H_20130504

#include<iostream>
#include<climits>
#include<vector>
#include<string>

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

void create_training_data();

std::vector< std::string > gen_msg();

std::vector< std::string > gen_msg2();

void show_msg(const std::vector< std::string > msg);
#endif
