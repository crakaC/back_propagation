#include<string>
#include<fstream>
#include<cmath>
#include"bp.h"

void set_data(params& param, std::vector< TrainingData >& target, const std::string filename = "training2.dat")
{
	using namespace std;
	ifstream ifs;
	ifs.open(filename.c_str());

	//ファイルを開けなかったら戻る。
	if(!ifs.is_open()){
		cout <<"cannot open " << filename << endl;
		return;
	}

	//データからパラメータを読み込む
	ifs >> param.num_sample >> param.num_input >> param.num_output;

	//訓練データをtargetに格納
	for(int isample = 0; isample < param.num_sample ; isample++){
		target.push_back(TrainingData(param.num_input, param.num_output));
		for(int i = 0; i < param.num_input; i++){
			ifs >> target[isample].input[i];
		}
		for(int i = 0; i < param.num_output; i++){
			ifs >> target[isample].output[i];
		}
	}

	//訓練データを表示
	printf("input training data from %s\n",filename.c_str());
	for(int isample = 0; isample < param.num_sample; isample++){
		cout <<"訓練データ No."<< isample+1 << endl;
		cout <<"入力：";
		int size = (int) target[isample].input.size();
		for( int i = 0; i < size; i++){
			cout << target[isample].input[i];
			if(i < size - 1){
				cout << ' ';
			}
		}
		cout << endl << "出力：";
		size = (int) target[isample].output.size();
		for(int i = 0; i < size; i++){
			cout << target[isample].output[i];
			if(i < size - 1){
				cout << ' ';
			}
		}
		cout << endl;
	}
}

void learn(const params param, const std::vector< TrainingData >& target, Net& net)
{
	//結線の重みを初期化
	net.init_weight();
	//各素子を初期化
	net.init_node();
	//学習
	net.learn(target);
}

void execute(const params param, Net net)
{
	std::map< Pair, double > w1 = net.w1, w2 = net.w2;
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
