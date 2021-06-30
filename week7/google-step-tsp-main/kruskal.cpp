#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

struct Edge{
    int start,end;
    double distance;
};

struct Tree{
    int node;
    std::vector<int> connected_node;
};

struct UnionFind{
    std::vector<int> parent;//parent[i]:iの親の番号。根なら木のサイズ*(-1)
    UnionFind(int n=0):parent(n,-1) {}//全ての木のサイズが１と初期.n点
    
    int find(int x){//根を返す
        if(parent[x] < 0) return x;//xが根の時はxを返す
        return parent[x] = find(parent[x]);
    }

    bool unite(int x,int y){//xとyの木をつなげたらtrue.大きい方に小さい方をつなげる.
        x = find(x);y = find(y);//根を求める
        if(x==y) return false;//同じ木に属していたらつなげる必要なし
        if(parent[x]>parent[y]) std::swap(x,y);//yの方がサイズが大きい時は交換する
        parent[x] += parent[y];//サイズの更新
        parent[y] = x;//根の更新
        return true;
    }

    bool same(int x,int y){return find(x)==find(y);}//xとyが同じ木にあるか

    int size(int x){return -parent[find(x)];}//xの木のサイズを返す
};

bool Compare(const Edge& e1,const Edge& e2){
    return e1.distance < e2.distance;
}

std::vector<Tree> Kruskal(std::vector<Edge> edge_weight, int number_of_cities){//,int number_of_edges){  
    UnionFind uf(number_of_cities);
    std::vector<Tree> smallest_tree(number_of_cities);
    for(int i = 0; i < number_of_cities;i++){
        smallest_tree[i].node = i;
    }
    std::sort(edge_weight.begin(),edge_weight.end(),Compare);
    /*for(Edge edge: edge_weight){
        std::cout<<edge.start<<" "<<edge.end<<" "<<edge.distance <<std::endl;
    }*/
    for(Edge edge: edge_weight){
        //std::cout<<edge.start<<" "<<edge.end<<" "<<edge.distance <<std::endl;
        if(!uf.same(edge.start,edge.end)){
            //std::cout<<"not same"<<std::endl;
            uf.unite(edge.start,edge.end);
            smallest_tree[edge.start].connected_node.emplace_back(edge.end);
            smallest_tree[edge.end].connected_node.emplace_back(edge.start);
        }
    }
    return smallest_tree;
}

void MakeRouteByDFS(int current_node,std::vector<Tree> smallest_tree,std::vector<int>& route,std::vector<bool>& seen){

    route.emplace_back(current_node);
    for(int next_node: smallest_tree[current_node].connected_node){
        if(!seen[next_node]){
            seen[next_node] = true;
            MakeRouteByDFS(next_node,smallest_tree,route,seen);
        }
    }
    route.emplace_back(current_node);
    return;
}
    
std::vector<int> MakeTour(int start,std::vector<int>& route,std::vector<bool>& seen){
    std::vector<int> first_tour;
    for(int node: route){
        if(!seen[node]){
            first_tour.emplace_back(node);
            seen[node] = true;
        }
    }
    return first_tour;
}

std::vector<int> MakeFirstTour(std::vector<Tree> smallest_tree){
    std::vector<int> route,first_tour;
    std::vector<bool> seen(smallest_tree.size());
    MakeRouteByDFS(0,smallest_tree,route,seen);
    seen.assign(smallest_tree.size(),false);
    first_tour = MakeTour(0,route,seen);
    return first_tour;
}


/*
int main(){
    std::cin>>V>>E;
    std::vector<Edge>edge_weight(E);
    for (int i = 0; i < E; i++)
    {
        int s,t;double w;
        std::cin>>s>>t>>w;
        Edge e = {s,t,w};
        edge_weight[i] = e;
    }
    std::vector<Tree> ans=Kruskal(edge_weight,V,E);
    for(int i = 0; i < V;i++){
        std::cout<<i<<std::endl;
        for(auto n_node:ans[i].connected_node){
            std::cout<<n_node<<" ";
        }
        std::cout<<std::endl;
    }

    std::vector<int> tour = MakeFirstTour(ans);
    return 0;
}
*/