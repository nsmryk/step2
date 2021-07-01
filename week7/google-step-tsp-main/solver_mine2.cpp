#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include "common.cpp"
#include "kruskal.cpp"
#include "solver_ant.cpp"
#include <ctime>
#include <random>

double GetScore(vector<int> tour){
    double score = 0.0;
    std::cout<<tour.size();
    for(int i = 0; i < tour.size();i++){
        std::cout<<i;
        score += Distance(cities[tour[i]],cities[tour[(i+1)%tour.size()]]);
    }
    return score;
}

//makes edge_weight, vector of distance 
std::vector<Edge> GetDistanceVector(std::vector<City> cities){
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
    return edge_weight;
}

// Returns true if two paths are crossed. path1:city_1-city_2, path2: city_3-city_4
bool Is_cross(City city_1,City city_2,City city_3,City city_4){
    double before_distance = Distance(city_1,city_2) + Distance(city_3,city_4);
    double after_distance = Distance(city_1,city_3) + Distance(city_2,city_4);
    return before_distance > after_distance;
}

//Returns true if city_3 should be inserted between city_1 and city_2

bool Is_shorter(City city_1,City city_2,City city_3,City city_4,City city_5){
    double before_distance = Distance(city_1,city_2) + Distance(city_3,city_4)+Distance(city_4,city_5);
    double after_distance = Distance(city_1,city_4) + Distance(city_2,city_4)+Distance(city_3,city_5);
    return before_distance > after_distance;
}

// Uncross paths, 
//tour[0]...tour[index_left]...tour[index_right]...tour[-1] => tour[0]...tour[index_right]...tour[index_left]...tour[-1]
void UncrossPath(std::vector<int>& tour, int index_left, int index_right){
    std::vector<int> tour_to_reverse = {tour.begin()+index_left+1,tour.begin()+index_right+1};
    std::reverse(tour_to_reverse.begin(),tour_to_reverse.end());
    for(int i = index_left+1;i < index_right+1; i++){
        tour[i] = tour_to_reverse[i-index_left-1];
    }
    return;
}

// Updates tour 
//tour[0]...tour[index_to],tour[index_to+1]...tour[index_insert-1],tour[index_insert],tour[index_insert+1]...tour[-1]
// => tour[0]...tour[index_to],tour[index_insert],tour[index_to+1]...tour[index_insert-1],tour[index_insert+1]...tour[-1]
void InsertNode(std::vector<int>& tour,int index_to, int index_insert){
    int insert_number = tour[index_insert];
    tour.erase(tour.begin()+index_insert);
    if(index_to<=index_insert){
        tour.insert(tour.begin()+index_to,insert_number);
    }else{
        tour.insert(tour.begin()+index_to-1,insert_number);
    }
    return;
}

bool ImproveTour_2opt(std::vector<City> cities, std::vector<int>& tour){
    //std::cout<<"imprive is called"<<std::endl;
    bool is_improved = false,is_updated = false;
    int number_of_cities = cities.size();
    while(true){
       // std::cout<<"in while"<<std::endl;
        is_improved = false;
        for(int i = 0; i < number_of_cities-2;i++){
            for(int j = i+2;j<number_of_cities;j++){
                //std::cout<<i<<" "<<j<<std::endl;
                if( i == 0 && j == number_of_cities-1 ){
                    continue;
                }
                if( Is_cross(cities[tour[i]],cities[tour[i+1]],cities[tour[j]],cities[tour[(j+1)%number_of_cities]])){
                    //std::cout<<"cross "<<i<<" "<<j<<std::endl;
                    UncrossPath(tour,i,j);
                    is_improved = true;
                }
            }
        }
        if(!is_improved){
            break;
        }else{
            is_updated = true;
        }
    }
    return is_updated;
}

// Or-opt( k = 1 )
bool ImproveTour(std::vector<City> cities, std::vector<int>& tour){
    //std::cout<<"imprive is called"<<std::endl;
    bool is_improved=false,is_updated=false;
    int number_of_cities = cities.size();
    while(true){
       // std::cout<<"in while"<<std::endl;
        is_improved = false;
        for(int i = 0; i < number_of_cities-1;i++){
            for(int j = i+2;j<number_of_cities+3;j++){
                if( i == j%number_of_cities || i == (j+1)%number_of_cities ){
                    continue;
                }
                //std::cout<<i<<" "<<j<<std::endl;
                if( Is_shorter(cities[tour[i]],cities[tour[i+1]],cities[tour[j%number_of_cities]],cities[tour[(j+1)%number_of_cities]],cities[tour[(j+2)%number_of_cities]]) ){
                    //std::cout<<"cross "<<i<<" "<<j<<std::endl;
                    InsertNode(tour,i+1,(j+1)%number_of_cities);
                    is_improved = true;
                }
            }
        }
        if(!is_improved){
            break;
        }else{
            is_updated = true;
        }
    }
    return is_updated;
}


// Makes tour, answer of TSP 
std::vector<int> solve(std::vector<City> cities){
    
    std::vector<Edge> edge_weight = GetDistanceVector(cities);
    std::vector<Tree> smallest_tree = Kruskal(edge_weight,cities.size());
    std::vector<int> tour = MakeFirstTour(smallest_tree,cities);
    
    bool is_updated= true;
    while(is_updated){
        is_updated = false;
        is_updated = ImproveTour_2opt(cities,tour);
        is_updated = is_updated || ImproveTour(cities,tour);
        
    }
    return tour;
}

std::vector<int> solve_use_ant(std::vector<City> cities){
    std::vector<int> shortest_tour(cities.size());
    bool is_updated= true;
    double min_score = 1000000;
    //for(int loop = 0;loop<10;loop++){
        std::vector<int> tour = solve_ant(cities);
        while(is_updated){
            is_updated = false;
            is_updated = ImproveTour_2opt(cities,tour);
            is_updated = is_updated || ImproveTour(cities,tour);
        }
    //    std::cout<<loop<<"is done"<<std::endl;
    //    double score = GetScore(tour);
    /*    if(min_score > score){
            std::cout<<"shorter"<<std::endl;
            min_score = score;
            shortest_tour = tour;
        }
    }*/
    return tour;
}
int main(){
    
    std::vector<City> cities = ReadInput("input_"+std::to_string(0)+".csv");
    /*std::vector<int> tour = {1,2,3,4,5,6,7,8,9};
    int index_left = 0,index_right=1;
    UncrossPath(tour, index_left, index_right);
    for(auto t:tour){
        std::cout<<t<<" ";
    }
    */
    int CHALLENGES = 8;
    for(int i = 6; i < CHALLENGES;i++){
        std::vector<City> cities = ReadInput("input_"+std::to_string(i)+".csv");
        std::vector<int> tour = solve_use_ant(cities);
        std::ofstream ofs( "output_"+std::to_string(i)+".csv" );
        ofs << FormatTour(tour);
    }
    
    return 0;
}