#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

struct City {
    double x_coordinate,y_coordinate;
};

std::vector<City> ReadInput(std::string filename){
    std::ifstream file( filename );
    std::string xy_coordinates;
    City city;
    std::vector<City> cities;
    while (std::getline(file, xy_coordinates)) {
        if(xy_coordinates == "x,y") continue;
        auto index = xy_coordinates.find(',');
        city.x_coordinate = stod(xy_coordinates.substr(0, index));
        city.y_coordinate = stod(xy_coordinates.substr(index + 1));
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