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

int main()
{
	using namespace std;

	//表示テキスト
	const vector< string > msg = gen_msg(), conf_msg = gen_msg2();
	vector< TrainingData > target;
	Params p;
	Net net( &p );
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
			set_data( p, target, filename );
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
				key = input_key< int >( 0, (int)conf_msg.size() - 1 );
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
					cout << "（ ＾ω＾）戻るお。" << endl;
				}
			}
			key = -1;
			break;
		case 6:
			//訓練データ作成
			create_training_data();
			break;
		case 0:
			cout << "（ ＾ω＾）終了するお。" << endl;
			break;
		}
	}
}
