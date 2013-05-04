#include<string>
#include<fstream>
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
