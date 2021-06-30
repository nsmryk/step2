#include<iostream>
#include<vector>
#include<string.h>
#include<algorithm>
#include<fstream>
#include<random>
#include <time.h>
#include <array>
const int ITERATION_TIMES = 10;
const int ARRANGEMENTS = 6;
const int MATRIX_SIZE = 800;
static std::mt19937_64 mt64(100);

    // [min_val, max_val] の一様分布整数 (int) の分布生成器
std::uniform_int_distribution<uint64_t> get_rand_uni_int( 1, 10 );

//std::random_device rd;
//std::mt19937 mt(rd());

void GenerateMatrix(int matrix[][MATRIX_SIZE]){
    for(int i = 0; i < MATRIX_SIZE; i++){
        for(int j = 0; j < MATRIX_SIZE; j++){
            matrix[i][j] = get_rand_uni_int(mt64);
        }
    }
}
double MeasureTimeOfMultiplyingMatrix(int matrix_a[][MATRIX_SIZE] ,int matrix_b[][MATRIX_SIZE], int matrix_c[][MATRIX_SIZE],int order){
    clock_t start,end;
    switch (order){
        case 0: //ijk
            start = clock();
            for(int i = 0; i < MATRIX_SIZE; i++){
                for(int j = 0; j < MATRIX_SIZE; j++){
                    for(int k = 0; k < MATRIX_SIZE; k++){
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            end = clock();
            break;
        case 1: //ikj
            start = clock();
            for(int i = 0; i < MATRIX_SIZE; i++){
                for(int k = 0; k < MATRIX_SIZE; k++){
                    for(int j = 0; j < MATRIX_SIZE; j++){
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            end = clock();
            break;
        case 2: //jik
            start = clock();
            for(int j = 0; j < MATRIX_SIZE; j++){
                for(int i = 0; i < MATRIX_SIZE; i++){
                    for(int k = 0; k < MATRIX_SIZE; k++){
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            end = clock();
            break;
        case 3: //jki
            start = clock();
            for(int j = 0; j < MATRIX_SIZE; j++){
                for(int k = 0; k < MATRIX_SIZE; k++){
                    for(int i = 0; i < MATRIX_SIZE; i++){
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            end = clock();
            break;
        case 4: //kij
            start = clock();
            for(int k = 0; k < MATRIX_SIZE; k++){
                for(int i = 0; i < MATRIX_SIZE; i++){
                    for(int j = 0; j < MATRIX_SIZE; j++){
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            end = clock();
            break;
        default: //kji
            start = clock();
            for(int k = 0; k < MATRIX_SIZE; k++){
                for(int j = 0; j < MATRIX_SIZE; j++){
                    for(int i = 0; i < MATRIX_SIZE; i++){
                        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
                    }
                }
            }
            end = clock();
            break;
    }
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double CalcurateAverage(double arr[], int size){
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += arr[i];
    }
    return sum/(double)size;
}

double CalcurateVariance(double arr[], int size, double average){
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += (arr[i]-average)*(arr[i]-average);
    }
    return sqrt(sum/(double)size);
}

int main(){
    int matrix_a[MATRIX_SIZE][MATRIX_SIZE],matrix_b[MATRIX_SIZE][MATRIX_SIZE],matrix_c[MATRIX_SIZE][MATRIX_SIZE];
    
    double duration[ARRANGEMENTS][ITERATION_TIMES];

    for(int loop = 0; loop < ITERATION_TIMES; loop++){

        GenerateMatrix(matrix_a);
        GenerateMatrix(matrix_b);
        
        for(int order = 0; order < ARRANGEMENTS;order++){
            memset(matrix_c,0,sizeof(matrix_c));
            duration[order][loop] += MeasureTimeOfMultiplyingMatrix( matrix_a, matrix_b, matrix_c, order);
        }
    }
    
    std::cout<<MATRIX_SIZE<<std::endl;
    for(int order = 0; order < ARRANGEMENTS;order++){
        double duration_average = CalcurateAverage(duration[order],ITERATION_TIMES);
        double duration_variance = CalcurateVariance(duration[order],ITERATION_TIMES, duration_average);
        printf("%lf %lf\n",duration_average,duration_variance);
        //printf("duration average : %lf duration variance : %lf\n",duration_average,duration_variance);
    }
    
    return 0;
}