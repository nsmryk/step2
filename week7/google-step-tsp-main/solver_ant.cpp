//Ant algorithms for Travelling salesman Problem

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <fstream>
#include "common.cpp"
//#include <cv.h>
//#include <highgui.h>

#define MAX_CITIES 8192
#define MAX_DIST 100000
#define MAX_TOUR (MAX_CITIES * MAX_DIST)
#define MAX_ANTS 30

using namespace std;

struct AntType{
	
	int curCity, nextCity, pathIndex;
	int tabu[MAX_CITIES];
	int path[MAX_CITIES];
	double tourLength;
};

//Ant algorithm problem parameters
#define ALPHA 1.0
#define BETA 5.0 //This parameter raises the weight of distance over pheromone
#define RHO 0.5 //Evapouration rate
#define QVAL 100
#define MAX_TOURS 20
#define MAX_TIME (MAX_TOURS * MAX_CITIES)
#define INIT_PHER (1.0/MAX_CITIES)

//runtime Structures and global variables

vector<City> cities; //(MAX_CITIES);
AntType ants[MAX_ANTS];

double distance_matrix[MAX_CITIES][MAX_CITIES];

double phero[MAX_CITIES][MAX_CITIES];

double best=(double)MAX_TOUR;
int bestIndex;

// Sets ants and pheromon of each ants
void Init(vector<City> cities)
{
	int from,to,ant;
    int number_of_cities = cities.size();
	//computing distance and phero
	for(from = 0; from < number_of_cities; from++)
	{
		for( to = from; to < number_of_cities; to++)
		{
            distance_matrix[from][to] = Distance(cities[from],cities[to]);
            distance_matrix[to][from] = distance_matrix[from][to];
            phero[from][to] = INIT_PHER;
            phero[to][from] = INIT_PHER;
		}
	}
	//initializing the ANTs
	
	for( ant = 0; ant < MAX_ANTS; ant++){

		if(to == number_of_cities){
            to=0;
        }	
		ants[ant].curCity = to++;
		
		for(from = 0; from < number_of_cities; from++){
			ants[ant].tabu[from] = 0;
			ants[ant].path[from] = -1;
		}
		
		ants[ant].pathIndex = 1;
		ants[ant].path[0] = ants[ant].curCity;
		ants[ant].nextCity = -1;
		ants[ant].tourLength = 0;
		
		//loading first city into tabu list
		
		ants[ant].tabu[ants[ant].curCity] =1;
		
	}
}

//Reinitializes all ants and redistributes them
void RestartAnts(int number_of_cities){
	int ant,i,to=0;
	
	for(ant = 0; ant<MAX_ANTS; ant++){
		if(ants[ant].tourLength < best){
			best = ants[ant].tourLength;
			bestIndex = ant;
		}
		ants[ant].nextCity = -1;
		ants[ant].tourLength = 0.0;

		for(i=0;i<number_of_cities;i++){
			ants[ant].tabu[i] = 0;
			ants[ant].path[i] = -1;
		}
		
		if(to == number_of_cities){
            to=0;
        }	
		ants[ant].curCity = to++;
		
		ants[ant].pathIndex = 1;
		ants[ant].path[0] = ants[ant].curCity;
		
		ants[ant].tabu[ants[ant].curCity] = 1;
	}
}

// Calculates probability
double AntProduct(int from, int to){
	return(( pow( phero[from][to], ALPHA) * pow( (1.0/ distance_matrix[from][to]), BETA)));
}

// Selects next city to visit according to probability
int SelectNextCity( int ant ,int number_of_cities ){
	int from, to;
	double denom = 0.0;
	
	from=ants[ant].curCity;
	
	for(to=0;to<number_of_cities;to++){
		if(ants[ant].tabu[to] == 0){
			denom += AntProduct( from, to );
		}
	}
	
	assert(denom != 0.0);
	
	do{
		double p;
		to++;
		
		if(to >= number_of_cities)
			to=0;
		if(ants[ant].tabu[to] == 0)
		{
			p = AntProduct(from,to)/denom;
			
			double x = ((double)rand()/RAND_MAX); 
			if(x < p){
				break;
			}
		}
	}while(1);
	
	return to;
}
	
// Moves ants according to probability
int SimulateAnts(int number_of_cities)
{
	int k;
	int moving = 0;
	for(k=0; k<MAX_ANTS; k++)
	{
		//checking if there are any more cities to visit
		
		if( ants[k].pathIndex < number_of_cities )
		{
			ants[k].nextCity = SelectNextCity(k,number_of_cities);
			ants[k].tabu[ants[k].nextCity] = 1;
			ants[k].path[ants[k].pathIndex++] = ants[k].nextCity;
			
			ants[k].tourLength += distance_matrix[ants[k].curCity][ants[k].nextCity];
			
			//handle last case->last city to first
			
			if(ants[k].pathIndex == number_of_cities)
			{
				ants[k].tourLength += distance_matrix[ants[k].path[number_of_cities -1]][ants[k].path[0]];
			}
			
			ants[k].curCity = ants[k].nextCity;
			moving++;
			
		}
	}
	
	return moving;
}

//Updates trails
void UpdateTrails(int number_of_cities)
{
	int from,to,i,ant;
	
	//Pheromone Evaporation
	
	for(from=0; from<number_of_cities;from++)
	{
		for(to=0;to<number_of_cities;to++)
		{
			if(from!=to)
			{
				phero[from][to] *=( 1.0 - RHO);
				
				if(phero[from][to]<0.0)
				{
					phero[from][to] = INIT_PHER;
				}
			}
		}
	}
	
	//Add new pheromone to the trails
	
	for(ant=0;ant<MAX_ANTS;ant++)
	{
		for(i=0;i<number_of_cities;i++)
		{	
			if( i < number_of_cities-1 )
			{
				from = ants[ant].path[i];
				to = ants[ant].path[i+1];
			}
			else
			{
				from = ants[ant].path[i];
				to = ants[ant].path[0];
			}
			
			phero[from][to] += (QVAL/ ants[ant].tourLength);
			phero[to][from] = phero[from][to];
			
		}
	}
	
	for (from=0; from < number_of_cities;from++)
	{
		for( to=0; to<number_of_cities; to++)
		{
			phero[from][to] *= RHO;
		}
	}
	
}

// Solve TSP by ACO
vector<int> SolveAnt(vector<City> cities){
    std::cout<<"ACO is called"<<std::endl;
    int curTime = 0;
    int number_of_cities = cities.size();
    best=(double)MAX_TOUR;
 	srand(time(NULL));
	vector<int>tour(number_of_cities);
	Init(cities);
    std::cout<<"init"<<std::endl;
	while( curTime++ < MAX_TIME)
	{
		if( SimulateAnts(number_of_cities) == 0)
		{
			UpdateTrails(number_of_cities);
			
			if(curTime != MAX_TIME)
				RestartAnts(number_of_cities);
				
			cout<<"\n Time is "<<curTime<<"("<<best<<")";
			
		}
	}
    AntType antBest;
	antBest = ants[bestIndex];
	for(int i=0;i<number_of_cities;i++)
	{
	    tour[i] = antBest.path[i];
	}
    return tour;
}
