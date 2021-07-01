#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
//#include "common.cpp"
#include "kruskal.cpp"

#define INF 1e7

double dp[1<<MAXN][MAXN];

//Calculates distance between city1 and city2
double Distance(City city1, City city2){
    return sqrt(pow((city1.x_coordinate - city2.x_coordinate), 2) + pow((city1.y_coordinate - city2.y_coordinate), 2));
}

void MakeDistanceMatrix(double distance_matrix[][MAXN],std::vector<City> cities){
    for(int i = 0; i < MAXN-1;i++){
        for(int j = 0;j < MAXN;j++){
            distance_matrix[i][j] = Distance(cities[i],cities[j]);
        }
    }
    return; 
}

double rec(int S, int v, std::vector<int>& tour, double distance_matrix[][MAXN]) {
    if(dp[S][v] >= 0) return dp[S][v];
    if(S==(1<<n)-1 && v==0) return dp[S][v]=0;
    double min_cost=INF;
    for(int u = 0; u<n; u++){
        if(!(S>>u & 1)){
            double tour_cost = rec( S|1<<u,u,tour,distance_matrix ) + distance_matrix[v][u];
            if(min_cost > tour_cost ){
                min_cost = tour_cost;
            }
            tour.emplace
        } 
    }
    return dp[S][v]=min_cost;
}

int main() {
    int CHALLENGES = 1;
    for(int i = 0; i < CHALLENGES;i++){
        std::vector<City> cities = ReadInput("input_"+std::to_string(i)+".csv");
        
        const int MAXN = cities.size();
        std::fill((double *)dp, (double *)(dp+(1<<MAXN)), -1);
        double distance_matrix[cities.size()][cities.size()];
        MakeDistanceMatrix(distance_matrix,cities);
        std::vector<int> tour = rec((1<<cities.size())-1,0);
        std::ofstream ofs( "output_"+std::to_string(i)+".csv" );
        ofs << FormatTour(tour);
    }
    std::fill((double *)dp, (double *)(dp+(1<<MAXN)), -1);
    std::cout << rec(0,0) << std::endl;
    return 0;
}










//Ant algorithms for Travelling salesman Problem

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <fstream>
//#include <cv.h>
//#include <highgui.h>

#define MAX_CITIES 30
#define MAX_DIST 100
#define MAX_TOUR (MAX_CITIES * MAX_DIST)
#define MAX_ANTS 30

using namespace std;

//Initial Definiton of the problem
struct City{
	int x,y;
};

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

City cities[MAX_CITIES];
AntType ants[MAX_ANTS];

double distance_matrix[MAX_CITIES][MAX_CITIES];

double phero[MAX_CITIES][MAX_CITIES];

double best=(double)MAX_TOUR;
int bestIndex;

//function init() - initializes the entire graph
double Distance(City city1, City city2){
    return sqrt(pow((city1.x - city2.x), 2) + pow((city1.y - city2.y), 2));
}
void init()
{
	int from,to,ant;
    
	//computing distance and phero
	for(from = 0; from < MAX_CITIES; from++)
	{
		for( to = from; to < MAX_CITIES; to++)
		{
            phero[from][to] = INIT_PHER;
            phero[to][from] = INIT_PHER;
            distance_matrix[from][to] = Distance(cities[from],cities[to]);
            distance_matrix[to][from] = distance_matrix[from][to];
		}
	}
	//initializing the ANTs
	
	to = 0;
	for( ant = 0; ant < MAX_ANTS; ant++){
		if(to == MAX_CITIES){
            to=0;
        }
		ants[ant].curCity = to++;
		
		for(from = 0; from < MAX_CITIES; from++){
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

//reinitialize all ants and redistribute them
void restartAnts(){
	int ant,i,to=0;
	
	for(ant = 0; ant<MAX_ANTS; ant++){
		if(ants[ant].tourLength < best){
			best = ants[ant].tourLength;
			bestIndex = ant;
		}
		ants[ant].nextCity = -1;
		ants[ant].tourLength = 0.0;

		for(i=0;i<MAX_CITIES;i++){
			ants[ant].tabu[i] = 0;
			ants[ant].path[i] = -1;
		}
		
		if(to == MAX_CITIES){
            to=0;
        }	
		ants[ant].curCity = to++;
		
		ants[ant].pathIndex = 1;
		ants[ant].path[0] = ants[ant].curCity;
		
		ants[ant].tabu[ants[ant].curCity] = 1;
	}
}

double antProduct(int from, int to){
	return(( pow( phero[from][to], ALPHA) * pow( (1.0/ distance_matrix[from][to]), BETA)));
}

int selectNextCity( int ant ){
	int from, to;
	double denom = 0.0;
	
	from=ants[ant].curCity;
	
	for(to=0;to<MAX_CITIES;to++){
		if(ants[ant].tabu[to] == 0){
			denom += antProduct( from, to );
		}
	}
	
	assert(denom != 0.0);
	
	do{
		double p;
		to++;
		
		if(to >= MAX_CITIES)
			to=0;
		if(ants[ant].tabu[to] == 0)
		{
			p = antProduct(from,to)/denom;
			
			//printf("\n%lf %lf", (double)rand()/RAND_MAX,p);
			double x = ((double)rand()/RAND_MAX); 
			if(x < p){
				break;
			}
		}
	}while(1);
	
	return to;
}
	
int simulateAnts()
{
	int k;
	int moving = 0;
	
	for(k=0; k<MAX_ANTS; k++)
	{
		//checking if there are any more cities to visit
		
		if( ants[k].pathIndex < MAX_CITIES )
		{
			ants[k].nextCity = selectNextCity(k);
			ants[k].tabu[ants[k].nextCity] = 1;
			ants[k].path[ants[k].pathIndex++] = ants[k].nextCity;
			
			ants[k].tourLength += distance_matrix[ants[k].curCity][ants[k].nextCity];
			
			//handle last case->last city to first
			
			if(ants[k].pathIndex == MAX_CITIES)
			{
				ants[k].tourLength += distance_matrix[ants[k].path[MAX_CITIES -1]][ants[k].path[0]];
			}
			
			ants[k].curCity = ants[k].nextCity;
			moving++;
			
		}
	}
	
	return moving;
}

//Updating trails

void updateTrails()
{
	int from,to,i,ant;
	
	//Pheromone Evaporation
	
	for(from=0; from<MAX_CITIES;from++)
	{
		for(to=0;to<MAX_CITIES;to++)
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
		for(i=0;i<MAX_CITIES;i++)
		{	
			if( i < MAX_CITIES-1 )
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
	
	for (from=0; from < MAX_CITIES;from++)
	{
		for( to=0; to<MAX_CITIES; to++)
		{
			phero[from][to] *= RHO;
		}
	}
	
}

void emitDataFile(int bestIndex)
{
	ofstream f1;
	f1.open("Data.txt");
	AntType antBest;
	antBest = ants[bestIndex];
	//f1<<antBest.curCity<<" "<<antBest.tourLength<<"\n";
	int i;
	for(i=0;i<MAX_CITIES;i++)
	{
		f1<<antBest.path[i]<<" ";
	}
	
	f1.close();
	
	f1.open("city_data.txt");
	for(i=0;i<MAX_CITIES;i++)
	{
		f1<<cities[i].x<<" "<<cities[i].y<<"\n";
	}
	f1.close();
	
}
	


int main()
{
	int curTime = 0;
    
    cout<<"S-ACO:";
	cout<<"MaxTime="<<MAX_TIME;
	
	srand(time(NULL));
	
	init();
	
	while( curTime++ < MAX_TIME)
	{
		if( simulateAnts() == 0)
		{
			updateTrails();
			
			if(curTime != MAX_TIME)
				restartAnts();
				
			cout<<"\n Time is "<<curTime<<"("<<best<<")";
			
		}
	}
	
	cout<<"\nSACO: Best tour = "<<best<<endl<<endl<<endl;
	
	emitDataFile(bestIndex);
	
	return 0;
}
	