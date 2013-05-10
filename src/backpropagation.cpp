//Back Propagation

#include<iostream>
#include<string>
#include"mylib.hpp"
#include"Net.hpp"

int main()
{
	using namespace std;

	//表示テキスト
	const vector< string > msg = genMsg(), conf_msg = genMsg2();
	Net net = Net();
	int key = -1; //キー入力
	string filename;
	while( key != 0 ){
		showMsg( msg );
		key = inputByKb< int >( 0, (int)msg.size() - 1 );
		switch(key){
		case 1:
			//データ入力
			cout << "input file name > "<<flush;
			cin >> filename;
			net.setTrainingData( filename );
			break;
		case 2:
			//学習
			net.learnBatch();
			break;
		case 3:
			//試してみる
			testBackPropagation( &net );
			break;
		case 4:
			//パラメータの調整
			while( key != 0 ){
				showMsg( conf_msg );
				key = inputByKb< int >( 0, (int)conf_msg.size() - 1 );
				switch(key){
				case 1:
					setAllParams( &net );
					break;
				case 2:
					setHiddenNodesNum( &net );
					break;
				case 3:
					setLearnNum( &net );
					break;
				case 4:
					setThresholdError( &net );
					break;
				case 5:
					setSigmoidGain( &net );
					break;
				case 6:
					setLearningCoefficient( &net );
					break;
				case 0:
					cout << "（ ＾ω＾）戻るお。" << endl;
				}
			}
			key = -1;
			break;
		case 5:
			//訓練データ作成
			createTrainingData( &net );
			break;
		case 0:
			cout << "（ ＾ω＾）終了するお。" << endl;
			break;
		}
	}
}
