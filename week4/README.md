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
└── Wikipedia_find_shortest_path.cpp
```

## 実行方法
課題1のコードはWikipedia_find_shortest_path.cppでC++で書かれています。<br>
実行環境 g++ version 10.2.0<br>
step_wikipedia-graph内で下のコマンドを実行してください。
```
g++ Wikipedia_find_shortest_path.cpp && ./a.out
```

## プログラムの実行結果
実行結果は以下のようになります。
最初にGoogleから渋谷までの最短経路を表示して、次にそのコストを表示します。

```
The shortest path is following below
Google->セグウェイ->渋谷
the distance between Google and 渋谷 is 2
```

## プログラムの概要
### Googleから渋谷までの最短経路を見つける
`ReadPages`で pages.txt , `ReadLinks`で links.txt を読み込んでpages,linksに格納する。<br>
次に`FindTargetNode`でGoogleと渋谷のページIDを探す。<br>
`FindShortestPathByBFS `でキューを使ってBFSで最短経路を探す。`is_way_found`で最短経路が見つかったかを管理する。経路を保存するために`record_previous_node`を使って
それぞれのページidに対してそれを訪れた前のページidを保存する。終点を見つけたら`is_way_found=true`にしてループを抜ける。<br>
経路が見つからなかったら`There is no way to 渋谷`を出力。
見つかったならば`TranslateMapToVector`で経路を`vector`にして格納し、それを使って`OutputResult`で最短経路とその距離を出力する。

## プログラム内の関数の説明
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