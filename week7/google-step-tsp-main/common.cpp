#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

struct City {
    double x,y;
};

std::vector<City> ReadInput(std::string filename){
    std::ifstream file( filename );
    std::string xys;
    City city;
    std::vector<City> cities;
    while (std::getline(file, xys)) {
        if(xys == "x,y") continue;
        auto index = xys.find(',');
        city.x = stod(xys.substr(0, index));
        city.y = stod(xys.substr(index + 1));
        cities.emplace_back(city);
    }
    std::cout<<filename<<" is read"<<std::endl;
    return cities;
}

std::string FormatTour( std::vector<int> tour ){
    std::string formatted_tour = "index";
    for(int city: tour){
        formatted_tour += "\n" + std::to_string(city);
    }
    return formatted_tour;
}

void PrintTour( std::vector<int> tour ){
    printf("%s",FormatTour(tour));
    return;
}
//Calculates distance between city1 and city2
double Distance(City city1, City city2){
    return sqrt(pow((city1.x - city2.x), 2) + pow((city1.y - city2.y), 2));
}