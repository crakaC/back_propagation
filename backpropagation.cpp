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
	const string MSG[3] ={
		"1.訓練データ入力\n2.学習\n3.試してみる\n4.パラメータ調整\n0.終了",
		"hoge",
		"piyo"
	};
	vector< TrainingData > target;
	params p;
	Net net(&p);
	int key = -1; //キー入力
	while(key != 0){
		cout <<MSG[0] << endl;
		key = input_key<int>(0,4);
		switch(key){
		case 1:
			//データ入力
			set_data(p, target,"training2.dat");
			break;
		case 2:
			//学習
			net.learn( target );
			break;
		case 3:
			//試してみる
			execute(p, net);
			break;
		case 4:
			//パラメータの調整
			config(&p);
			break;
		case 0:
			cout <<"終了します"<<endl;
			break;
		}
	}
}
