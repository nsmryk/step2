# Real malloc challenge!
## 実行方法 
下のコマンドを`/real-malloc`で実行するとベンチマークが実行されます。

```
make
make run
```

## 実装したもの
malloc.cでsimple_malloc.cと同じ関数、構造体を導入し、その中のfirst-fitに対して新たにbest-fit, worst-fitを追加しました。
best-fitでは空き容量を全探索して追加したいデータのサイズ以上の空き容量の中で最小のものを使います。
逆にworst-fitは全探索して追加したいデータのサイズ以上の空き容量の中でサイズが最大のものを選ぶようにしています。

## 実行結果
下の図のようになりました。
simple\_malloc.c => malloc.c 
|   | Challenge 1 | Challenge 2 | Challenge 3 | Challenge 4 | Challenge 5 |
|:-|:-|:-|:-|:-|:-|
| first-fit  | Time: 14 ms => 13 mn<br>Utilization: 70% => 70%  | Time: 9 ms => 7 ms<br>Utilization: 40% => 40%  |  Time: 146 ms => 145 ms<br>Utilization: 8% => 7% | Time: 28944 ms => 30344 ms<br>Utilization: 15% => 15%  | Time: 19608 ms => 21939 ms<br>Utilization: 15% => 15%  |
| best-fit  | Time: 10 ms => 1199 ms<br>Utilization: 70% => 70%  | Time: 6 ms => 527 ms<br>Utilization: 40% => 40%  | Time: 137 ms => 684 ms<br>Utilization: 8% => 50%  | Time: 23937 ms => 7984 ms<br>Utilization: 15% => 71%  | Time: 17045 ms => 5219 ms<br>Utilization: 15% => 74%  |
| worst-fit  | Time: 13 ms => 1179 ms<br>Utilization: 70% => 69%  | Time: 6 ms => 564 ms<br>Utilization: 40% => 40%  |  Time: 135 ms => 58889 ms<br>Utilization: 8% => 3% | Time: 26234 ms => 894827 ms<br>Utilization: 15% => 6%  | Time: 22730 ms => 776806 ms<br>Utilization: 15% => 6%  |