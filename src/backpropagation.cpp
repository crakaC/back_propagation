//Back Propagation
//訓練データの形式として
//1行目に、データ数、入力数、出力数を記述
//2行目以降、入力値、出力値をスペースまたは改行で区切って記述。
#include<iostream>
#include<string>
#include"mylib.h"
#include"Net.h"

int main()
{
	using namespace std;

	//表示テキスト
	const vector< string > msg = gen_msg(), conf_msg = gen_msg2();
	Net net = Net();
	int key = -1; //キー入力
	string filename;
	while( key != 0 ){
		show_msg( msg );
		key = input_key< int >( 0, (int)msg.size() - 1 );
		switch(key){
		case 1:
			//データ入力
			cout << "input file name > "<<flush;
			cin >> filename;
			net.set_training_data( filename );
			break;
		case 2:
			//学習
			net.learn();
			break;
		case 3:
			//試してみる
			test_bp( &net );
			break;
		case 4:
			while( key != 0 ){
				show_msg( conf_msg );
				key = input_key< int >( 0, (int)conf_msg.size() - 1 );
				switch(key){
				case 1:				//パラメータの調整(一括)
					set_all_params( &net );
					break;
				case 2:
					set_hidden_nodes_num( &net );
					break;
				case 3:
					set_learn_num( &net );
					break;
				case 4:
					set_threshold_error( &net );
					break;
				case 5:
					set_s_gain( &net );
					break;
				case 6:
					set_epsilon( &net );
					break;
				case 0:
					cout << "（ ＾ω＾）戻るお。" << endl;
				}
			}
			key = -1;
			break;
		case 5:
			//訓練データ作成
			create_training_data();
			break;
		case 0:
			cout << "（ ＾ω＾）終了するお。" << endl;
			break;
		}
	}
}
