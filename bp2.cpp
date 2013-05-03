//誤差逆伝搬法プログラムその2
//Programmed by Kosuke SHIRAKASHI
//Shima Lab. 2013
//訓練データの形式として
//1行目に、データ数、入力数、出力数
//2行目以降、入力値、出力値をスペースまたは改行で区切って記述。


#include<iostream>
#include<cmath>
#include<fstream>
#include<climits>
#include<cstdlib>
#include<string>
#include<vector>
#include<map>

typedef std::pair<int,int> Pair;

//訓練データ
struct TrainingData{
	std::vector<int> input;
	std::vector<int> output;
	TrainingData(int i, int o){
		input = std::vector<int>(i);
		output = std::vector<int>(o);
	}
};

//各パラメータ
struct params{
	int num_learn, num_sample;//学習打ち切り回数、訓練データ数
	int num_input, num_hidden, num_output;//入力素子数、隠れ層素子数、出力素子数
	double s_gain, epsilon;//シグモイド関数ゲイン、学習重み
	double threshold_error;//許容誤差
	params(){
		num_learn = 10000;
		num_hidden = 10;
		s_gain = 1.0;
		epsilon = 0.05;
		threshold_error = 0.01;
	}
};

//訓練データ格納
std::vector< TrainingData > target;
//各素子の値を突っ込む。
std::vector< double > x, h, y;
//結線の重み。
std::map< Pair, double > w1, w2;
//逆伝搬量
std::vector<double> h_back, y_back;
//シグモイド関数
double sigmoid( const double s , const double gain){
	return 1 - (1 / ((double)1 + exp(gain * s)));
}

//-1から1の範囲の乱数を返す。
double d_rand(){
	double ret = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	return (rand()%2 == 0) ? ret : -ret;
}


const std::string INTRODUCTION = "****************************************\n誤差逆伝搬法プログラム\nProgrammed by Kosuke SHIRAKASHI\nSHIMA Lab. 2013\n****************************************\n";

const std::string MSG[3] ={
	"1.データ入力\n2.学習\n0.終了",
	"hoge",
	"piyo"
};

void set_data(params& param, const std::string filename = "training2.dat")
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

void learn(const params param)
{
	const int n_input = param.num_input, n_hidden = param.num_hidden, n_output = param.num_output;
	const int n_learn = param.num_learn, n_sample = param.num_sample;
	const double gain = param.s_gain, epsilon = param.epsilon, threshold_error = param.threshold_error;

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

	//vector初期化
	x.assign(n_input + 1, 0.0);
	h.assign(n_hidden + 1, 0.0);
	y.assign(n_output, 0.0);

	y_back.assign(n_output,0.0);
	h_back.assign(n_hidden,0.0);

	//学習
	for(int ilearn = 0; ilearn < n_learn; ilearn++){
		double max_error = 0;

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
			printf("学習回数 = %d, 訓練データNO.%d, 誤差 = %lf\n", ilearn, isample+1, error);

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

		//誤差が十分小さくなったらループを抜ける。
		if( max_error < threshold_error ){
			break;
		}
	}
}

//キー入力でうまいことやる関数。
template<class T> T input_key(T begin, T end){
	T value;
	std::cin >> value;
	while( !std::cin || (value < begin || end < value)){
		std::cout << begin <<"から"<<end<<"の間で入力することを強いられているんだ…！"<<std::endl;
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
		std::cout << '>'<<std::flush;
		std::cin >> value;
	}
	return value;
}

int main()
{
	using namespace std;
	params p;
	int key = -1; //キー入力
	while(key != 0){
		//system("reset");//windowsならsystem("cls")
		cout << INTRODUCTION << endl;
		cout <<MSG[0] << endl;
		key = input_key<int>(0,2);
		switch(key){
		case 1:
			//データ入力
			cout << "input data from file." << endl;
			set_data(p);
			break;
		case 2:
			//学習
			cout <<"learn and optimize weight of node connections"<< endl;
			learn(p);
			break;
		case 3:
			break;
		case 0:
			cout <<"終了します"<<endl;
			break;
		}
		cout <<"何かキー入力してエンタープリーズ."<<endl;
		char c;
		cin >> c;
	}
}
