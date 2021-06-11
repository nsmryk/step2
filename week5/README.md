課題のURL
<https://github.com/nsmryk/google-step-tsp/blob/main/solver_mine.py>

# 宿題5
solver_mine.py に実装されている

## 実行方法
```python3 solver_mine.py```

```python3 -m http.server```

を実行した後、<http://localhost:8000/visualizer/build/default/>でビジュアル化した結果を見ることができる。

##　実行結果
Input_{i}.csvの結果はOutput_{i}.csv(i = 0,1...6)に出力されている。
上から順に訪れていく。
greedyアルゴリズムで求められた経路から交わっているパスをなくしているのでgreedyアルゴリズムで得られた結果より向上されている。

