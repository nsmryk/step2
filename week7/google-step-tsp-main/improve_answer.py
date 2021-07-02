#!/usr/bin/env python3
import time
import sys
import math

from common import read_input, format_tour, read_tour


def distance(city1, city2):
    return math.sqrt((city1[0] - city2[0]) ** 2 + (city1[1] - city2[1]) ** 2)


def expanded_distance_squared(city1, city2, expansion_rate):
    """
    Calculate the distance after expanding in the vertical direction
    expansion_rate: Expansion rate in the vertical direction
    """
    return (city1[0] - city2[0]) ** 2 + ((city1[1] - city2[1])*expansion_rate) ** 2


def total_distance(tour, dist):
    """ Calculate the total distance of tour """
    total = 0
    for index in range(len(tour)):
        total += dist[tour[index-1]][tour[index]]
    return total


def greedy(cities, expansion_rate):

    N = len(cities)

    dist = [[0] * N for _ in range(N)]
    for i in range(N):
        for j in range(i, N):
            dist[i][j] = dist[j][i] = expanded_distance_squared(
                cities[i], cities[j], expansion_rate)

    current_city = 0
    unvisited_cities = set(range(1, N))
    tour = [current_city]

    while unvisited_cities:
        next_city = min(unvisited_cities,
                        key=lambda city: dist[current_city][city])
        unvisited_cities.remove(next_city)
        tour.append(next_city)
        current_city = next_city
    return tour


def two_opt(tour, dist):
    """
    Swap two edges a-b,c-d to a-c,b-d if the total distance becomes shorter
    dist: Array of distances between two cities
    """
    start_time = time.time()
    N = len(tour)
    while True:
        count = 0  # Number of times tour was changed
        for a_index in range(N-2):
            b_index = a_index+1
            for c_index in range(a_index+2, N):
                d_index = (c_index+1) % N
                a = tour[a_index]
                b = tour[b_index]
                c = tour[c_index]
                d = tour[d_index]
                if dist[a][b]+dist[c][d] > dist[a][c]+dist[b][d]:
                    tour[b_index:c_index +
                         1] = reversed(tour[b_index: c_index+1])
                    count += 1

        if count == 0:
            break
    end_time = time.time()
    print("Two_opt has finished. \nTime: {}[s] Total distance: {}".format(
        round(end_time-start_time, 1), total_distance(tour, dist)))
    return tour


def n_or_opt(tour, dist, subsequence_length):
    """
    Move subsequence e from between a and b to between c and d
    if the total distance becomes shorter
    dist: Array of distances between two cities
    subsequence_length: The length of e
    """
    start_time = time.time()
    N = len(tour)
    while True:
        count = 0  # Number of times tour was changed
        for a_index in range(N-1):
            b_index = (a_index+subsequence_length+1) % N
            e_indexes = [(a_index+1+i) % N for i in range(subsequence_length)]
            for c_index in range(N-1):
                d_index = (c_index+1) % N

                # When c,d are not included in e_indexes
                if d_index not in e_indexes and d_index != b_index:
                    a = tour[a_index]
                    b = tour[b_index]
                    c = tour[c_index]
                    d = tour[d_index]
                    e = [tour[index] for index in e_indexes]
                    if dist[a][e[0]]+dist[e[-1]][b]+dist[c][d] > dist[a][b]+dist[c][e[-1]]+dist[e[0]][d]:
                        # Replace the cities of e with -1. The indexes of c and d are not changed
                        for i in range(subsequence_length):
                            tour[(a_index+1+i) % N] = -1
                        # Reverse e and put e between c and d
                        for i in range(subsequence_length):
                            tour.insert(d_index, e[i])
                        # Remove the cities of e by removing -1
                        for i in range(subsequence_length):
                            tour = [city for city in tour if city != -1]
                        count += 1
        if count == 0:
            break
    end_time = time.time()
    print("{}-or-opt has finished. \nTime: {} [s] Total distance: {}".format(subsequence_length,
                                                                             round(end_time-start_time, 1), total_distance(tour, dist)))
    return tour


if __name__ == '__main__':
    #assert len(sys.argv) > 2
    cities = read_input("input_7.csv")
    N = len(cities)

    dist = [[0] * N for _ in range(N)]
    for i in range(N):
        for j in range(i, N):
            dist[i][j] = dist[j][i] = distance(cities[i], cities[j])
    print("Distance array was created.")

    if N == 2048:
        expansion_rate = 1.15
        tour = read_tour("output_6.csv")
        for _ in range(4):
            tour = two_opt(tour, dist)
            tour = n_or_opt(tour, dist, 7)
            tour = n_or_opt(tour, dist, 6)
            tour = n_or_opt(tour, dist, 5)
            tour = n_or_opt(tour, dist, 4)
            tour = n_or_opt(tour, dist, 3)
            tour = n_or_opt(tour, dist, 2)
            tour = n_or_opt(tour, dist, 1)
    elif N == 8192:
        expansion_rate = 1.0
        tour = read_tour("output_7.csv")
        tour = two_opt(tour, dist)
        tour = n_or_opt(tour, dist, 3)
        tour = n_or_opt(tour, dist, 2)
        tour = n_or_opt(tour, dist, 1)
        tour = two_opt(tour, dist)

    with open(f'output_7.csv', 'w') as f:
            f.write(format_tour(tour) + '\n')
    