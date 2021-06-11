#!/usr/bin/env python3
import numpy as np
import sys
import math
import copy

from common import print_tour, read_input, format_tour
INF = 100100
def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0]) ** 2 + (city1[1] - city2[1]) ** 2)

def get_distance(cities):
    number_of_cities = len(cities)
    distance_matrix = [[0] * number_of_cities for i in range(number_of_cities)]
    for i in range(number_of_cities):
        for j in range(i, number_of_cities):
            distance_matrix[i][j] = distance_matrix[j][i] = distance(cities[i], cities[j])
    return distance_matrix

def calculate_2opt_exchange_cost(tour, i, j, distance_matrix):
    """Calculate the difference of cost by applying given 2-opt exchange"""
    n_cities = len(tour)
    a, b = tour[i], tour[(i + 1) % n_cities]
    c, d = tour[j], tour[(j + 1) % n_cities]

    cost_before = distance_matrix[a, b] + distance_matrix[c, d]
    cost_after = distance_matrix[a, c] + distance_matrix[b, d]
    return cost_after - cost_before
def apply_2opt_exchange(visit_order, i, j):
    """Apply 2-opt exhanging on visit order"""

    tmp = visit_order[i + 1: j + 1]
    tmp.reverse()
    visit_order[i + 1: j + 1] = tmp

    return visit_order

def improve_with_2opt(visit_order, distance_matrix):
    """Check all 2-opt neighbors and improve the visit order"""
    n_cities = len(visit_order)
    cost_diff_best = 0.0
    i_best, j_best = None, None

    for i in range(0, n_cities - 2):
        for j in range(i + 2, n_cities):
            if i == 0 and j == n_cities - 1:
                continue

            cost_diff = calculate_2opt_exchange_cost(
                visit_order, i, j, distance_matrix)

            if cost_diff < cost_diff_best:
                cost_diff_best = cost_diff
                i_best, j_best = i, j

    if cost_diff_best < 0.0:
        visit_order_new = apply_2opt_exchange(visit_order, i_best, j_best)
        return visit_order_new
    else:
        return None 
def calculate_total_distance(order, distance_matrix):
    """Calculate total distance traveled for given visit order"""
    idx_from = np.array(order)
    idx_to = np.array(order[1:] + [order[0]])
    distance_arr = distance_matrix[idx_from, idx_to]

    return np.sum(distance_arr)
def local_search(visit_order, distance_matrix, improve_func):
    """Main procedure of local search"""
    cost_total = calculate_total_distance(visit_order, distance_matrix)

    while True:
        improved = improve_func(visit_order, distance_matrix)
        if not improved:
            break

        visit_order = improved

    return visit_order
def greedy(dist,city_list):
    current_city = city_list.pop(0)
    unvisited_cities = city_list#set(range(1, number_of_cities))
    tour = [current_city]
    while unvisited_cities:
        next_city = min(unvisited_cities,
                        key=lambda city: dist[current_city][city])
        
        unvisited_cities.remove(next_city)
        tour.append(next_city)
        current_city = next_city
    return tour

  
if __name__ == '__main__':
    # assert len(sys.argv) > 1
    CHALLENGES = 7
   # cities = read_input(f'input_{2}.csv')
   # tour = solve(cities)
    
    for i in range(CHALLENGES):
        #print(i)
        cities = read_input(f'input_{i}.csv')
        dist = get_distance(cities)
        city_list = list(range(0, len(cities)))
        tour = greedy(dist,city_list)
        improved = local_search(tour, dist, improve_with_2opt)
        #print_tour(tour)
        with open(f'output_{i}.csv', 'w') as f:
            f.write(format_tour(tour) + '\n')
            
        