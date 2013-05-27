#include<cstdio>
#include<climits>
#include<string>
#include<iostream>
#include<fstream>
#include"mylib.hpp"

//訓練データを作成する
void createTrainingData( BackPropagation* bp )
{
	using namespace std;
	string filename;
	ofstream ofs;
	int sample_num, input_num, output_num;
	cout << "output file name\n>" << flush;
	cin >> filename;

	//ファイルをオープン
	ofs.open( filename.c_str() );
	if( !ofs.is_open() ){
		cout <<"cannot create " << filename << '.' << endl;
		return;
	}

	//訓練データ数、入力数、出力数を入力
	cout << "sample number" << flush;
	sample_num = inputByKb< int >( 1, INT_MAX - 1 );
	cout << "input number " << flush;
	input_num = inputByKb< int >( 1, INT_MAX - 1 );
	cout << "output number " << flush;
	output_num = inputByKb< int >( 1, INT_MAX - 1 );

	//流しこむ
	ofs << sample_num << ' ' << input_num << ' ' << output_num << endl;

	//データを作成
	for(int i = 0; i < sample_num; i++){
		double* p = new double[ input_num  ];
		for(int j = 0; j < input_num; j++){
			printf("[%d/%d]input%d\n", i + 1, sample_num, j + 1 );
			p[j] = inputByKb< double >( -1, 1 );
		}
		for(int j = 0; j < input_num; j++){
			ofs << p[j] <<' ';
		}
		delete[] p;

		p = new double[ output_num ];
		for(int j = 0; j < output_num; j++){
			printf("[%d/%d]output%d\n", i + 1, sample_num, j + 1 );
			p[j] = inputByKb< double >( 0, 1 );
		}
		for(int j = 0; j < output_num; j++){
			if( j < output_num - 1 ){
				ofs << p[j] <<' ';
			}
			else{
				ofs << p[j] << endl;
			}
		}
		delete[] p;
	}

	printf( "データを作成しました(%s)\n"
			"読み込みますか？", filename.c_str() );

	if( inputYorN() == 'y'){
		bp->setTrainingData( filename );
	}

}

//入力データから実行
void testBackPropagation( BackPropagation* bp ){
	char key = -1;
	do{
		if( !executeBackPropagation( bp )){
			return;
		}
		printf( "もう一度試しますか？" );
		key = inputYorN();
	}while( key == 'y' );

}

//キーボードからデータを入力して実行
bool executeBackPropagation( BackPropagation* bp )
{
	int num_input = bp->getInputNum();

	if( !( bp->isTrained() ) ){
		printf( "先に訓練データを学習してください\n" );
		return false;
	}
	//各素子の値を入れるためのやつ
	std::vector< double > input, output;

	printf( "input values.\n" );
	for( int i = 0; i < num_input; i++ ){
		double value;
		printf( "[%d/%d]\n", i + 1, num_input );
		value = inputByKb< double >( 0, 1 );
		input.push_back( value );
	}
	input.push_back( 1.0 ); //閾値

	//出力を求める。
	output = bp->output( input );

	//表示
	printf( "-------出力-------\n" );
	for( int i = 0; i < (int)output.size(); i++ ){
		printf( "%d:%G\n", i+1, output[i] );
	}
	printf( "------------------\n" );

	return true;
}

char inputYorN()
{
	using std::cin;
	char c;
	printf("[y/n] > ");
	cin >> c;
	while( !cin || ( c != 'y'  && c != 'n' ) ){
		printf( "y か n で入力することを強いられているんだ…！\n" );
		cin.clear();
		cin.ignore( INT_MAX, '\n' );
		printf("[y/n]? > ");
		cin >> c;
	}
	return c;
}
//コマンドライン用メッセージその１
std::vector< std::string > genMsg()
{
	std::vector< std::string > msg;
	msg.push_back("訓練データ入力");
	msg.push_back("学習");
	msg.push_back("試してみる");
	msg.push_back("パラメータ調整");
	msg.push_back("訓練データ作成");
	msg.push_back("終了");

	return msg;
}

//コマンドライン用メッセージその２
std::vector< std::string > genMsg2()
{
	std::vector< std::string > msg;
	msg.push_back("一括パラメータ調整");
	msg.push_back("中間層数設定");
	msg.push_back("中間層素子数設定");
	msg.push_back("学習回数設定");
	msg.push_back("許容誤差設定");
	msg.push_back("ゲイン設定");
	msg.push_back("学習係数設定");
	msg.push_back("戻る");

	return msg;
}

//コマンドライン用メッセージを表示
void showMsg(const std::vector< std::string > msg)
{
	int i;
	for( i = 0; i < (int)msg.size() - 1; i++ ){
		std::cout << i+1 << ':' << msg[i] << std::endl;
	}
	std::cout << "0:" << msg[i] << std::endl;
}

//学習回数を設定
void setLearnNum( BackPropagation* bp )
{
	printf( "学習回数(現在%d) ", bp->getLearnNum() );
	bp->setLearnNum( inputByKb<int>( 1, INT_MAX - 1 ) );
}

//パラメータ設定
void setAllParams( BackPropagation* bp )
{
	setHiddenLayerNum( bp );
	setHiddenNodesNum( bp );
	setLearnNum( bp );
	setThresholdError( bp );
	setSigmoidGain( bp );
	setLearningCoefficient( bp );
}

//中間層素子数設定
void setHiddenNodesNum( BackPropagation* bp )
{
	printf( "中間層素子数(現在%d) ", bp->getHiddenNodesNum() );
	bp->setHiddenNodesNum( inputByKb<int>( 1, 100 ) );
}

//中間層数設定
void setHiddenLayerNum( BackPropagation* bp )
{
	printf( "中間層数(現在%d) ", bp->getHiddenLayerNum() );
	bp->setHiddenLayerNum( inputByKb<int>( 1, 100 ) );
}


//シグモイド関数ゲインの設定
void setSigmoidGain( BackPropagation* bp )
{
	printf( "シグモイド関数ゲイン(現在%G) ", bp->getSigmoidGain() );
	bp->setSigmoidGain( inputByKb<double>( -10.0, 10.0 ) );
}

//学習係数の設定
void setLearningCoefficient( BackPropagation* bp )
{
	printf( "学習係数ε(現在%G) ", bp->getLearningCoefficient() );
	bp->setLearningCoefficient( inputByKb<double>( 0, 10.0 ) );
}

//許容誤差の設定
void setThresholdError( BackPropagation* bp )
{
	printf( "許容誤差(現在%G) ", bp->getThresholdError() );
	bp->setThresholdError( inputByKb<double>( 1e-60, 1 ) );
}
