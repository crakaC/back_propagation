##なにこれ

誤差逆伝播法（Back Propagation）のプログラムです。  
中間層（隠れ層）の数を自由に変更できるようになりました。

##How to use

1. cloneするか、zipで落として適当なディレクトリに展開してください。
2. makeすれば実行ファイルができます。
3. 煮るなり焼くなりご自由にどうぞ。

Windowsの場合はソースをVisualStudioやEclipseに突っ込んでプロジェクトの文字コードをUTF8に設定すれば多分動くと思いますが動かなかったらごめんなさい。

###訓練データの作成

まずは適当に訓練データ（教師信号）を作成しましょう．
- sample number:サンプル数
- input number:訓練データの入力数
- output number:訓練データの出力数

１以上の整数ならある程度大きくても実行出来ますが，手で訓練データを入力するのは大変なので適当にやっちゃってください．  

入力、出力は0~1の範囲で選択出来ます．  

訓練データの書式ですが、  
サンプル数３，入力数２，出力数１なら、

```cpp
3 2 1 //左からサンプル数、入力数、出力数
0 0 0 //訓練データ1 入力(0, 0) 出力(0)
0 1 1 //訓練データ2 入力(0, 1) 出力(1)
1 0 1 //訓練データ3 入力(1, 0) 出力(1)
```

というようなデータが作成されます。こういう形式になっていれば、どんなエディタを使って作成しても構いません。  
改行は無視するので入力、出力の間に改行を入れて見やすくできます。

###訓練データの読み込み
訓練データを作成した場合は、そのあと読み込むかどうか聞いてくるので読み込みましょう。  
すでにデータを作ってある場合は、ファイル名を入力すればOKです。

###学習
逐次更新学習と一括更新学習があります。  
訓練データの数が多いと，なかなか誤差が小さくならないです。  
一括更新学習の方が収束しやすいですが、少し時間がかかります。  
うまくいかないときはパラメータを変更してみてください。

###試す
学習かうまくいったかどうか、任意の入力を与えて試せます。  
未知の入力を与えた時、汎化ができているかどうかを確認しましょう。

###パラメータの編集
デフォルトではすべての訓練データと出力の値の２乗誤差が1E-05(0.00001)を下回るか，  
50000回の学習で打ち切るようになっています．  
中間層（隠れ層）の素子数の初期値は10です．  
他にもパラメータはありますが，まとめて変更できるので自由に試してみてください．

###学習推移グラフ
学習時の誤差をファイルに出力しているので、gnuplotを使ってgraph.pltファイルをロードすると誤差の推移を表示できます。  

```
gnuplot  
load "graph.plt"
```

###未実装
- 学習後に中間層の素子を削れるようにする。
- 学習の途中で学習係数を変更する。
- 重みを修正するときに慣性項を使う。
- リアルタイムで学習推移をグラフ化
- パターン認識

実装できるかどうかはやる気と元気次第です．

##注意
一応動作確認はしていますが、実行して何かあっても自己責任でお願いします。  