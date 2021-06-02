# STEP&nbsp;Week4&nbsp;Homework
## 実行の準備
wikipedia_data.zip をダウンロードして解凍し、以下のようなディレクトリ構成にしてください。
ゆうきさんと同じ構成です。
<br>

```
step_wikipedia-graph
├── data
│   ├── graph_small.png
│   ├── links_small.txt
│   ├── links.txt
│   ├── pages_small.txt
│   └── pages.txt
├── .gitignore
├── README.md
├── Wikipedia_calculate_page_rank.cpp
└── Wikipedia_find_shortest_path.cpp
```
## Googleから渋谷までの最短経路を見つける

### 実行方法
課題1のコードはWikipedia_find_shortest_path.cppでC++で書かれています。<br>
実行環境 g++ version 10.2.0<br>
step_wikipedia-graph内で下のコマンドを実行してください。
```
g++ Wikipedia_find_shortest_path.cpp && ./a.out
```

### プログラムの実行結果
実行結果は以下のようになります。
最初にGoogleから渋谷までの最短経路を表示して、次にそのコストを表示します。

```
The shortest path is as follows
Google->セグウェイ->渋谷
the distance between Google and 渋谷 is 2
```

### プログラムの概要
`ReadPages`で pages.txt , `ReadLinks`で links.txt を読み込んでpages,linksに格納する。<br>
次に`FindTargetNode`でGoogleと渋谷のページIDを探す。<br>
`FindShortestPathByBFS `でキューを使ってBFSで最短経路を探す。`is_way_found`で最短経路が見つかったかを管理する。経路を保存するために`record_previous_node`を使って
それぞれのページidに対してそれを訪れた前のページidを保存する。終点を見つけたら`is_way_found=true`にしてループを抜ける。<br>
経路が見つからなかったら`There is no way to 渋谷`を出力。
見つかったならば`TranslateMapToVector`で経路を`vector`にして格納し、それを使って`OutputResult`で最短経路とその距離を出力する。

### プログラム内の関数の説明
- `ReadPages`<br>
    引数:なし<br>
    返り値:pages(map<string, string>)<br>
    １行ずつ読み込んでpagesに格納して返す。pagesはkeyがページid,値がページ名のmap。<br>
- `ReadLinks`<br>
    引数:なし<br>
    返り値:links(map<string,set<string>>)<br>
    ReadPages()と同様にファイルを読み込む。mapのlinksに格納して返す。linksのkeyはページidで値はそのページから出ているリンク先のページidのset。<br>
- `FindTargetNode`<br>
    引数: target_name(string, idを知りたいページ名) pages(map<string, string>, ReadPages()で読み込んだもの)<br>
    返り値: target_id (string, target_nodeに対するページid)<br>
    pages内をtarget_nameと一致するまで全探索する。
- `FindShortestPathByBFS `<br>
    引数:start_point_id, end_point_id(string), links(map<string,set<string>>)<br>
    返り値:record_previous_node(map<string,string>)<br>
    BFSで最短路を求める。その際にそれぞれのページidをkeyとしてそれを訪れた前のページidをrecord_previous_nodeに保存する。
    経路が見つかれば探索を終了してrecord_previous_nodeを返す。見つからなければそれを判定するために以下のようにしてから`record_previous_node`を返す。
    ```
    record_previous_node[start_point_id] = -1;
    ```
    
- `TranslateMapToVector`<br>
    引数:start_point_id, end_point_id(string), pages(map<string,string>),record_previous_node(map<string,string>)<br>
    返り値:shortest_path(vector<string>)<br>
    `FindShortestPathByBFS `で求めたrecord_previous_nodeを終点から辿って経路として通るノードを順にshortest_pathに入れていく。
    shortest_pathの0ばんめの要素が終点、一番最後の要素に始点が入るように並ぶ。
- `OutputResult`<br>
    引数:start_point_id,end_point_id(string),pages,shortest_path(vector<string>)<br>
    返り値:なし<br>
    `shortest_path`では0番目に終点、一番最後の要素に始点が入るように並んでいるので最後の要素から順に出力していく。
    `distance`は経路の距離を意味している。

## ページランクの計算

### 実行方法
課題2のコードはWikipedia_calculate_page_rank.cppでC++で書かれています。<br>
実行環境 g++ version 10.2.0<br>
step_wikipedia-graph内で下のコマンドを実行してください。
```
g++ Wikipedia_calculate_page_rank.cpp && ./a.out
```
### プログラムの実行結果
ページランクが高い順に10個ページの名前とページランクが以下のように出力されます。
```
1 : page name is 日本 PageRank is 751625
2 : page name is アメリカ合衆国 PageRank is 370941
3 : page name is 英語 PageRank is 305012
4 : page name is 地理座標系 PageRank is 261579
5 : page name is 東京都 PageRank is 232939
6 : page name is 俳優 PageRank is 135951
7 : page name is イギリス PageRank is 134006
8 : page name is インターネット・ムービー・データベース PageRank is 128966
9 : page name is レコードレーベル PageRank is 117482
10 : page name is フランス PageRank is 114324
```
### プログラムの概要
課題1と同様に`ReadPages`で pages.txt , `ReadLinks`で links.txt を読み込んでpages,linksに格納する。<br>
次にページidとページランキングをいれるためのpage_rankingを初期化する。ページランクの初期値はsmall_numberで設定する。<br>
page_id_to_iteratorに`MapPageIdToIterator`でページidをkeyとしてvalueがage_rankingの配列のイテレーターとなるようなpage_id_to_iterator
を作る。<br>
is_any_score_changedを収束判定に使い、これがtrueの間はスコアを更新し続ける。
`CalculatePageRank`で次のページランクを求めてnext_scoreにいれる。現在のスコアと`CheckIfScoreChanges`で比較する。<br>
収束したらpage_rankingを更新してページランクが高い順にソートする。<br>
最後に`OutputResult`で結果を出力する。

### プログラム内の関数の説明
- `InitializePageRanking`<br>
    引数:pages<br>
    返り値:page_ranking(vector<PageRanking>)<br>
    ページランクのスコアを初期化する。vectorの要素は構造体PageRankでページidとページランクを記録する。
-  `MapPageIdToIterator`<br>
    引数:page_ranking<br>
    返り値:page_id_to_iterator(std::map<std::string, int>)<br>
    ページidとそれに対応するpage_rankingの要素のイテレータを対応づけてpage_id_to_iteratorに保存する。
-  `CalculatePageRank`<br>
    引数:page_ranking, page_id_to_iterator,links<br>
    返り値:next_score(vector<double>)<br>
    次のスコアを求めてnext_scoreに入れて返す。それぞれのノードに対して分配するスコアを求め、next_scoreに足し合わせていく。
-  `CheckIfScoreChanges`<br>
    引数:current_score,next_score<br>
    返り値:is_any_score_changed(boolean)<br>
    全てのノードに対してスコアの変更前後でsmall_numberより大きく変わっているか見る。
    どれか一つでも大きく変わっていたらtrueを返す。
-  `OutputResult`<br>
    引数:page_ranking,pages<br>
    返り値:なし<br>
    プログラムの実行結果のような形式で結果を出力する。