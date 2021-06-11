#!/usr/bin/env python3

import sys
import math
import copy

from common import print_tour, read_input, format_tour
INF = 100100
def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0]) ** 2 + (city1[1] - city2[1]) ** 2)
'''
Makes matrix whose element means distance of cities using distance()
Input cities (list of xycoordinates)
Output distance_matrix (distance between two cities)
'''
def get_distance(cities):
    number_of_cities = len(cities)
    distance_matrix = [[0] * number_of_cities for i in range(number_of_cities)]
    for i in range(number_of_cities):
        for j in range(i, number_of_cities):
            distance_matrix[i][j] = distance_matrix[j][i] = distance(cities[i], cities[j])
    return distance_matrix
'''
Checks if two roads are crossing
Input xycoordinates of four cities
Output boolean crossing -> true / not crossing -> no
'''
def is_cross(city_a1, city_a2, city_b1, city_b2):
    # 繋ぎ変える前後で距離を比べて繋ぎ変えた方が短ければ道が交差していると判定
    before_change_distance = distance(city_a1,city_a2) + distance(city_b1,city_b2)
    after_change_distance = distance(city_a1,city_b1) + distance(city_b2,city_a2)
    if before_change_distance > after_change_distance :
        return True
    else:
        return False

'''
Makes new order of cities
Changes "start -> ... -> city_iterator1->[city_iterator1+1->...-> city_iterator2]->city_iterator2+1->...end"
to      "start -> ... -> city_iterator1->[city_iterator2->...-> city_iterator1+1]->city_itera]tor2+1->...end"
inside [] is reversed
Input tour ,city_iterator1, city_iterator2
Output tour(new order)
'''
def uncross_pathes(tour,city_iterator1, city_iterator2 ):
    tmp = tour[city_iterator1 + 1 : city_iterator2 + 1 ]
    tour[city_iterator1 + 1 : city_iterator2 + 1 ] = tmp[::-1]
    return tour
'''
Improves tour finding crossing pathes and uncross them
Input cities, tour
Output tour
'''
def improve_tour(cities,tour):
    number_of_cities = len(cities)
    while True:
        improved = False
        for i in range(0,number_of_cities-2):
            for j in range(i+2,number_of_cities):
                if i == 0 and j == number_of_cities - 1:
                    continue
                if is_cross(cities[tour[i]],cities[tour[i+1]],cities[tour[j]],cities[tour[(j+1)%number_of_cities]]):
                    tour = uncross_pathes(tour,i,j)
                    improved = True
        if not improved:
            break
    return tour

'''
Decides the order to visit by greedy algorithm
Input distance_matrix,city_list
Output tour
'''
def greedy(distance_matrix,city_list):
    current_city = city_list.pop(0)
    unvisited_cities = city_list
    tour = [current_city]
    while unvisited_cities:
        next_city = min(unvisited_cities,
                        key=lambda city: distance_matrix[current_city][city])
        
        unvisited_cities.remove(next_city)
        tour.append(next_city)
        current_city = next_city
    return tour
'''
Finds route without crossing path
Input cities (xycoordinates of each city))
Output tour(the order to visit)
'''
def solve(cities):
    distance_matrix = get_distance(cities)
    city_list = list(range(0, len(cities)))
    tour = greedy(distance_matrix,city_list)
    tour = improve_tour(cities,tour)
    return tour

if __name__ == '__main__':
    # assert len(sys.argv) > 1
    # there are 7 input files
    CHALLENGES = 7
    for i in range(CHALLENGES):
        cities = read_input(f'input_{i}.csv')
        tour = solve(cities)
        with open(f'output_{i}.csv', 'w') as f:
            f.write(format_tour(tour) + '\n')
            
        