/*****************************************************************************/
/*! @file	BackPropagation.hpp
	@brief	BackPropagationのヘッダ

	BackPropagationに関わる部分の定義

	@par	関連ファイル
		-# @ref	BackPropagation.cpp	実装部分
		-# @ref	BackPropagation_setter_and_getter.cpp 実装部分（setter および getter）
	@sa	BackPropagation.cpp	実装部分
	@sa	BackPropagation_setter_and_getter.cpp 実装部分（setter および getter）
	@par	更新履歴
		- 2012/05/04 Ver. 1.0
			- 基本的な機能の実装 (by Kosuke Shirakashi)
		- 2012/06/09 Ver. 1.1
			- 中間層数および中間層素子数を調整可能に (by Kosuke Shirakashi)
	@par Copyright
		&copy; SHIMA Lab. 2013
******************************************************************************/

#ifndef BackPropagation_H_20130504
#define BackPropagation_H_20130504

#include<vector>
#include<string>

/*!	@brief 訓練データの1サンプルを表現する */
struct TrainingData{
	std::vector< double > input; ///< 入力
	std::vector< double > output; ///< 出力

	/*!
		@brief コンストラクタ
		入力数，出力数に対応した訓練データの作成
		@param [in] i 訓練データ1サンプルあたりの入力数
		@param [in] o 訓練データ1サンプルあたりの出力数
	*/
	TrainingData(int i, int o);
};

/*!	@brief パラメータ */
struct Params{
	int num_learn;	//!< 学習打ち切り回数
	int num_sample;	//!< 訓練データ数
	int num_input;	//!< 入力素子数、
	int num_hidden;	//!< 隠れ層素子数
	int num_output;//!< 出力素子数
	int num_hidden_layer;//!< 隠れ層素子数
	double sigmoid_gain;//!< シグモイド関数ゲイン
	double learning_coefficient;//!< 学習係数
	double interia_coefficient;//!< 慣性係数
	double threshold_error;//!< 許容誤差
	/*! 
		@brief 学習済みかどうかチェックするフラグ
	*/
	bool is_trained;
	bool is_empty;//!< 訓練データは存在するか
	Params();
};

/*! @brief バックプロパゲーションに関わるクラス */
class BackPropagation{
public:
	/*!
		@brief コンストラクタ
		ノードの領域を確保する
	*/
	BackPropagation();
	/*! 
		@brief
			与えられた入力に対して結果を出力する
		@param [in] input 入力データ
		@return ニューラルネットの出力		
	*/
	std::vector< double > output( const std::vector< double >& input );
	
	/*! 
		@brief 訓練データをファイルから読み込みセットする
		@param [in] filename 訓練データファイル
		@note ファイルが存在しない場合は標準出力に注意を表示する
		@return なし
	*/
	void setTrainingData( const std::string filename );

	/*! 
		@brief 一括更新学習を行う．
		@pre	訓練データが読み込まれている必要がある．
		@return なし
	*/
	void learnBatch();

	/*! 
		@brief	逐次更新学習を行う．
		@pre	訓練データが読み込まれている必要がある．
		@return なし
	*/
	void learnOnline();

	/*! @name SetterandおよびGetter*/
	/* @{ */
	/*! 
		@brief	入力ノード数を返す
		@return	入力ノード数
	*/
	int getInputNum();

	/*! 
		@brief	入力ノード数セットする
		@param [in] n 入力ノード数
		@return なし
	*/
	void setHiddenNodesNum( int n );

	/*! 
		@brief	隠れ層ノード数を返す
		@return	隠れ層ノード数の現在値
	*/
	int  getHiddenNodesNum();

	/*! 
		@brief	隠れ層ノード数セットする
		@param [in] n 隠れ層ノード数
		@return なし
	*/
	void setHiddenLayerNum( int n );

	/*! 
		@brief	隠れ層数を返す
		@return	隠れ層数の現在値
	*/
	int getHiddenLayerNum( void );	

	/*! 
		@brief	打ち切り学習回数をセットする
		@param [in] n 打ち切り学習回数
		@return なし
	*/
	void setLearnNum( int n );

	/*! 
		@brief	打ち切り学習回数を返す
		@return	打ち切り学習回数の現在値
	*/
	int getLearnNum();

	/*! 
		@brief	許容誤差をセットする
		@param [in] err 許容誤差
		@return なし
	*/
	void setThresholdError( double err );

	/*! 
		@brief	許容誤差を返す
		@return	許容誤差の現在値
	*/
	double getThresholdError();

	/*! 
		@brief	シグモイド関数のゲインをセットする
		@param [in] gain シグモイド関数のゲイン
		@return なし
	*/
	void setSigmoidGain( double gain );

	/*! 
		@brief	シグモイド関数のゲインを返す
		@return	シグモイド関数のゲインの現在値
	*/
	double getSigmoidGain();

	/*! 
		@brief	学習係数をセットする
		@param [in] eps 学習係数
		@return なし
	*/
	void setLearningCoefficient( double eps );

	/*! 
		@brief	学習係数を返す
		@return	学習係数の現在値
	*/
	double getLearningCoefficient();
	/* @} */

	/*! 
		@brief	学習が行われたかどうかを返す
		@retval	true 学習済
		@retval false 未学習
	*/
	bool isTrained();
private:

	/*!
		@brief ノード数や重みの初期化を行う
		@note	訓練データが読み込まれていない場合は注意を表示して戻る
		@return なし
	*/
	void initialize();

	/*!
		@brief 入力ノードの値から，各層のノードの値を更新する
		@return なし
	*/
	void updateStateForward();

	/*!
		@brief 逆方向の動作を行う
		i_sample番目の訓練データと出力した値の差を計算し，逆方向に動作させる．
		@param [in] i_sample 訓練データサンプル番号
		@return なし
	*/
	void updateStateBackword( const int i_sample );

	/*!
		@brief 1訓練データあたりの重み修正量を計算する
		@return なし
	*/
	void calcBondsWeightVariation();

	/*!
		@brief 重みの修正量をリセットする
	*/
	void resetBondsWeightVariation();

	/*!
		@brief 重みの修正を行う
		
		@ref calcBondsWeightVariation() によって求めた修正量を，各結線重みに加える
		@note 
			逐次更新学習法では，1つの訓練データ毎に 修正量を求め，重みの最適化を行うが，<BR>
			一括更新学習法では，全訓練データの修正量を求めてから重みの最適化を行う．
		@return なし
	*/
	void optimizeBondsWeight();

	/*!
		@brief 誤差をチェックする
		訓練データと，出力した値の誤差をチェックする
		@param [in] target 訓練データ
		@return 誤差
	*/
	double checkError( const TrainingData& target );

	/*!
		@brief -1~1の範囲で乱数を返す
		重みの初期化等に使用する
		@retval [-1, 1]
	*/
	double dRand();

	/*!
		@brief シグモイド関数
		@param [in] inputs 入力
		@param [in] gain ゲイン
		@retval [-1, 1]
	*/
	double sigmoid( const double inputs, const double gain );


	std::vector< double > input_layer; //!< 入力層ノード
	std::vector< std::vector < double > > hidden_layer;//!< 隠れ層ノード
	std::vector< double > output_layer;//!< 出力層ノード

	std::vector< double > output_layer_back;//!< 逆方向動作時の出力層ノードが取る値
	std::vector< std::vector < double > > hidden_layer_back;//!< 逆方向動作時の隠れ層ノードが取る値

	typedef std::vector< std::vector< std::vector< double > > > Vector3d;//!< ただの別名．
	Vector3d bonds_weight;//!< ノード間の重み
	Vector3d bonds_weight_variation;//!< 重み修正量

	Params param;//!< 各種パラメータ
	Params param_bk; //!< 学習時の状態を保存する

	std::vector< TrainingData > target; //!< 訓練データの組
};

#endif
