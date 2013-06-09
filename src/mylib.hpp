#ifndef MYLIB_H_20130504
#define MYLIB_H_20130504

#include<iostream>
#include<climits>
#include<vector>
#include<string>
#include"BackPropagation.hpp"

//キー入力を指定した値の範囲に制限。
template<class T> T inputByKb( T min, T max ){
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

char inputYorN();

//訓練データの作成,読み込み
void createTrainingData( BackPropagation* bp );

//自分で値を入力して実行
void testBackPropagation( BackPropagation* bp );
bool executeBackPropagation( BackPropagation* bp );

//コマンドライン用メッセージ1
std::vector< std::string > genMsg();

//コマンドライン用メッセージ2
std::vector< std::string > genMsg2();

//コマンドライン用メッセージ表示
void showMsg(const std::vector< std::string > msg);

//ニューラルネット各種パラメータ設定
void setAllParams( BackPropagation* bp );
void setHiddenNodesNum( BackPropagation* bp );
void setHiddenLayerNum( BackPropagation* bp );
void setLearnNum( BackPropagation* bp );
void setThresholdError( BackPropagation* bp );
void setSigmoidGain( BackPropagation* bp );
void setLearningCoefficient( BackPropagation* bp );

#endif
