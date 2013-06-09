/*****************************************************************************/
/*! @mainpage
	ニューラルネットを構築し，バックプロパゲーションによる学習，
	汎化の確認等を行うことができる
	
	@par	更新履歴
		- 2012/05/04 Ver. 1.0
			- 基本的な機能の実装 (by Kosuke Shirakashi)
		- 2012/06/09 Ver. 1.1
			- 中間層数および中間層素子数を調整可能に (by Kosuke Shirakashi)
	
	@attention 
		環境によっては起動できない可能性がある．<BR>
		@ref BackPropagation::BackPropagation() 内の値を小さくするとよいかも．
	@par Copyright
		&copy; 2013 SHIMA Lab.
******************************************************************************/

/*****************************************************************************/
/*! @file	main.cpp
	@brief	main()

	コンソール上で対話的にバックプロパゲーションの動作を行う．

	@see	BackPropagation.cpp	実装部分
	@see	BackPropagation_setter_and_getter.cpp 実装部分（setter および getter）
	@par	更新履歴
		- 2012/05/04 Ver. 1.0
			- 基本的な機能の実装 (by Kosuke Shirakashi)
		- 2012/06/09 Ver. 1.1
			- 中間層数および中間層素子数を調整可能に (by Kosuke Shirakashi)
	@par Copyright
		&copy; 2013 SHIMA Lab.
******************************************************************************/

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
	bp.setTrainingData("xor.dat");
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
			printf("1.逐次更新\n2.一括更新\n3.両方\n0.戻る\n");
			key = inputByKb< int >( 0, 3 );
			switch( key ){
			case 1:
				bp.learnOnline();
				break;
			case 2:
				bp.learnBatch();
				break;
			case 3:
				bp.learnOnline();
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
					setHiddenLayerNum( &bp );
					break;
				case 3:
					setHiddenNodesNum( &bp );
					break;
				case 4:
					setLearnNum( &bp );
					break;
				case 5:
					setThresholdError( &bp );
					break;
				case 6:
					setSigmoidGain( &bp );
					break;
				case 7:
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
