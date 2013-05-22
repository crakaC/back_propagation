//Back Propagation

#include<iostream>
#include<cstdio>
#include<string>
#include"mylib.hpp"
#include"BackPropagation.hpp"

int main()
{
	using namespace std;

	//表示テキスト
	const vector< string > msg = genMsg(), conf_msg = genMsg2();
	BackPropagation bp = BackPropagation();
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
			bp.setTrainingData( filename );
			break;
		case 2:
			//学習
			//system("cls");
			printf("1.逐次更新\n2.一括更新\n0.戻る\n");
			key = inputByKb< int >( 0, 2 );
			switch( key ){
			case 1:
				bp.learnOnline();
				break;
			case 2:
				bp.learnBatch();
				break;
			default:
				key = -1;
				break;
			}
			break;
		case 3:
			//試してみる
			testBackPropagation( &bp );
			break;
		case 4:
			//パラメータの調整
			while( key != 0 ){
				showMsg( conf_msg );
				key = inputByKb< int >( 0, (int)conf_msg.size() - 1 );
				switch(key){
				case 1:
					setAllParams( &bp );
					break;
				case 2:
					setHiddenNodesNum( &bp );
					break;
				case 3:
					setLearnNum( &bp );
					break;
				case 4:
					setThresholdError( &bp );
					break;
				case 5:
					setSigmoidGain( &bp );
					break;
				case 6:
					setLearningCoefficient( &bp );
					break;
				case 0:
					cout << "（ ＾ω＾）戻るお。" << endl;
				}
			}
			key = -1;
			break;
		case 5:
			//訓練データ作成
			createTrainingData( &bp );
			break;
		case 0:
			cout << "（ ＾ω＾）終了するお。" << endl;
			break;
		}
	}
}
