#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include "common.cpp"
#include "kruskal.cpp"

double Distance(City city1, City city2){
    return sqrt(pow((city1.x_coordinate - city2.x_coordinate), 2) + pow((city1.y_coordinate - city2.y_coordinate), 2));
}

std::vector<Edge> GetDistance(std::vector<City> cities){
    int number_of_cities = cities.size();
    std::vector<Edge> edge_weight;
    for(int i = 0; i < number_of_cities-1;i++){
        for(int j = i+1;j < number_of_cities;j++){
            //std::cout<<"distance "<<i<< " "<<j<<std::endl;
            Edge edge;
            edge.start = i;edge.end = j;
            edge.distance = Distance(cities[i],cities[j]);
            edge_weight.emplace_back(edge);
            edge.start = j;edge.end = i;
            edge_weight.emplace_back(edge);
        }
    }
    //for(Edge edge: edge_weight){
    //    std::cout<<edge.start<<" "<<edge.end<<" "<<edge.distance <<std::endl;
    //}
    return edge_weight;
}
bool Is_cross(City city_1,City city_2,City city_3,City city_4){
    double before_distance = Distance(city_1,city_2) + Distance(city_3,city_4);
    double after_distance = Distance(city_1,city_3) + Distance(city_2,city_4);
    return before_distance > after_distance;
}

void UncrossPath(std::vector<int>& tour, int index_left, int index_right){
    std::vector<int> tour_to_reverse = {tour.begin()+index_left+1,tour.begin()+index_right+1};
    std::reverse(tour_to_reverse.begin(),tour_to_reverse.end());
    for(int i = index_left+1;i < index_right+1; i++){
        tour[i] = tour_to_reverse[i-index_left-1];
    }
    return;
}
void ImproveTour(std::vector<City> cities, std::vector<int>& tour){
    //std::cout<<"imprive is called"<<std::endl;
    bool improved;
    int number_of_cities = cities.size();
    while(true){
       // std::cout<<"in while"<<std::endl;
        improved = false;
        for(int i = 0; i < number_of_cities-2;i++){
            for(int j = i+2;j<number_of_cities;j++){
                //std::cout<<i<<" "<<j<<std::endl;
                if( i == 0 && j == number_of_cities-1 ){
                    continue;
                }
                if( Is_cross(cities[tour[i]],cities[tour[i+1]],cities[tour[j]],cities[tour[(j+1)%number_of_cities]])){
                    //std::cout<<"cross "<<i<<" "<<j<<std::endl;
                    UncrossPath(tour,i,j);
                    improved = true;
                }
            }
        }
        if(!improved){
            break;
        }
    }
    return;
}
std::vector<int> solve(std::vector<City> cities){
    std::vector<Edge> edge_weight = GetDistance(cities);
    
    std::vector<Tree> smallest_tree = Kruskal(edge_weight,cities.size());
    std::vector<int> tour = MakeFirstTour(smallest_tree);
    ImproveTour(cities,tour);
    return tour;
}

int main(){
    /*std::vector<int> tour = {1,2,3,4,5,6,7,8,9};
    int index_left = 0,index_right=1;
    UncrossPath(tour, index_left, index_right);
    for(auto t:tour){
        std::cout<<t<<" ";
    }
    */
    int CHALLENGES = 8;
    for(int i = 0; i < CHALLENGES;i++){
        std::vector<City> cities = ReadInput("input_"+std::to_string(i)+".csv");
        std::vector<int> tour = solve(cities);
        std::ofstream ofs( "output_"+std::to_string(i)+".csv" );
        ofs << FormatTour(tour);
    }
    
    return 0;
}