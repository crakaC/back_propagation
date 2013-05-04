//Back Propagation
//訓練データの形式として
//1行目に、データ数、入力数、出力数を記述
//2行目以降、入力値、出力値をスペースまたは改行で区切って記述。
#include<iostream>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include<map>
#include"struct.h"
#include"mylib.h"
#include"bp.h"

//結線の重み。
std::map< Pair, double > w1, w2;

void learn(const params param, const std::vector< TrainingData >& target)
{
	const int n_input = param.num_input, n_hidden = param.num_hidden, n_output = param.num_output;
	const int n_learn = param.num_learn, n_sample = param.num_sample;
	const double gain = param.s_gain, epsilon = param.epsilon, threshold_error = param.threshold_error;

	//各素子の値
	std::vector< double > x(n_input + 1), h(n_hidden + 1), y(n_output);
	//逆伝搬量
	std::vector< double > h_back(n_hidden), y_back(n_output);

	//結線の重みを初期化
	srand(time(NULL));
	for(int i = 0; i < n_input + 1; i++){
		for(int j = 0; j < n_hidden; j++){
			w1[ Pair(i,j) ] = d_rand();
		}
	}
	for(int i = 0; i < n_hidden + 1; i++){
		for(int j = 0; j < n_output; j++){
			w2[ Pair(i,j) ] = d_rand();
		}
	}

	//学習
	int ilearn;
	double max_error;
	for(ilearn = 0; ilearn < n_learn; ilearn++){
		max_error = 0;
		for(int isample = 0; isample < n_sample; isample++){
			//順方向の動作
			for(int i = 0; i < n_input; i++){
				x[i] = target[isample].input[i];
			}
			x[n_input] = 1.0; //閾値

			//隠れ素子値の計算
			for(int j = 0; j < n_hidden; j++){
				double net_input = 0;
				for(int i = 0; i < n_input + 1; i++){
					net_input += w1[ Pair(i,j) ] * x[i];
				}
				h[j] = sigmoid(net_input, gain);
			}

			//出力値の計算
			for(int j = 0; j < n_output; j++){
				double net_input = 0;
				for(int i = 0; i < n_hidden + 1; i++){
					net_input += w2[ Pair(i,j) ] * h[i];
				}
				y[j] = sigmoid(net_input, gain);
			}

			//誤差の評価
			double error = 0;
			for(int j = 0; j < n_output; j++){
				error += pow( (target[isample].output[j] - y[j]), 2 );
			}
			error = error / static_cast<double>( n_output );

			if( error > max_error ){
				max_error = error;
			}

			//printf("学習回数 = %d, 訓練データNO.%d, 誤差 = %lf\n", ilearn, isample+1, error);

			//逆方向の動作
			//出力層素子
			for(int j = 0; j < n_output; j++){
				y_back[j] = (y[j] - target[isample].output[j]) * (1.0 - y[j]) * y[j] * gain;
			}

			//隠れ層素子の逆伝搬時の動作
			for(int i = 0; i < n_hidden; i++){
				double net_input = 0;
				for(int j = 0; j < n_output; j++){
					net_input += w2[ Pair(i,j)] * y_back[j];
				}
				h_back[i] = net_input * (1.0 - h[i]) * h[i] * gain;
			}

			//重みの修正
			for(int i = 0; i < n_input + 1; i++){
				for(int j = 0; j < n_hidden; j++){
					w1[Pair(i,j)] -= epsilon * x[i] * h_back[j];
				}
			}
			for(int i = 0; i < n_hidden + 1; i++){
				for(int j = 0; j < n_output; j++){
					w2[Pair(i,j)] -= epsilon * h[i] * y_back[j];
				}
			}
		}

		//誤差が小さくなったらループを抜ける。
		if( max_error < threshold_error ){
			break;
		}
	}
	printf("学習回数:%d, 誤差:%lf\n",ilearn, max_error);
}

void execute(const params param)
{
	const int n_input = param.num_input, n_hidden = param.num_hidden, n_output = param.num_output;
	const double gain = param.s_gain;
	//各素子の値を入れるためのやつ
	std::vector< double > x(n_input + 1), h(n_hidden + 1), y(n_output);

	printf( "入力値はいかがなさいますか？(n = %d)\n>",n_input);
	for(int i = 0; i < n_input; i++){
		std::cin >> x[i];
		while(!std::cin){
			std::cout <<"ちゃんと入力しちくり〜\n>"<<std::flush;
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
			std::cin >> x[i];
		}
	}
	x[n_input] = 1.0; //閾値

	//出力を求める。学習のとこのコピペなのでまとめたい。
	//隠れ素子値の計算
	for(int j = 0; j < n_hidden; j++){
		double net_input = 0;
		for(int i = 0; i < n_input + 1; i++){
			net_input += w1[ Pair(i,j) ] * x[i];
		}
		h[j] = sigmoid(net_input, gain);
	}

	//出力値の計算
	printf("--出力--\n");
	for(int j = 0; j < n_output; j++){
		double net_input = 0;
		for(int i = 0; i < n_hidden + 1; i++){
			net_input += w2[ Pair(i,j) ] * h[i];
		}
		y[j] = sigmoid(net_input, gain);
		printf("%d:%lf\n", j+1, y[j]);
	}
	printf("--------\n");
}

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
			cout <<"learn and optimize weight of node connections"<< endl;
			learn(p, target);
			break;
		case 3:
			//試してみる
			execute(p);
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
