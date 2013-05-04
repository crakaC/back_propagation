//Back Propagation
//訓練データの形式として
//1行目に、データ数、入力数、出力数を記述
//2行目以降、入力値、出力値をスペースまたは改行で区切って記述。
#include<iostream>
#include<string>
#include"struct.h"
#include"mylib.h"
#include"bp.h"
#include"Net.h"

std::vector< std::string > gen_msg()
{
	std::vector< std::string > msg;
	msg.push_back("訓練データ入力");
	msg.push_back("学習");
	msg.push_back("試してみる");
	msg.push_back("一括パラメータ調整");
	msg.push_back("パラメータ調整");
	msg.push_back("終了");

	return msg;
}

std::vector< std::string > gen_msg2()
{
	std::vector< std::string > msg;
	msg.push_back("隠れ層素子数設定");
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

int main()
{
	using namespace std;

	//表示テキスト
	const vector< string > msg = gen_msg(), conf_msg = gen_msg2();
	vector< TrainingData > target;
	Params p;
	Net net( &p );
	int key = -1; //キー入力
	while( key != 0 ){
		show_msg( msg );
		key = input_key< int >( 0, (int)msg.size() );
		switch(key){
		case 1:
			//データ入力
			set_data( p, target, "training2.dat" );
			break;
		case 2:
			//学習
			net.learn( target );
			break;
		case 3:
			//試してみる
			execute( p, net );
			break;
		case 4:
			//パラメータの調整(一括)
			p.set_all();
			break;
		case 5:
			while( key != 0 ){
				show_msg( conf_msg );
				key = input_key< int >( 0, (int)conf_msg.size() );
				switch(key){
				case 1:
					p.set_hidden_num();
					break;
				case 2:
					p.set_learn_num();
					break;
				case 3:
					p.set_threshold_error();
					break;
				case 4:
					p.set_s_gain();
					break;
				case 5:
					p.set_epsilon();
					break;
				case 0:
					cout << "戻るお。（ ＾ω＾）" << endl;
				}
			}
			key = -1;
		case 0:
			cout << "終了します" << endl;
			break;
		}
	}
}
