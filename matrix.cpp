#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<fstream>
#include<random>
#include <time.h>
int N;
static std::mt19937_64 mt64(100);

    // [min_val, max_val] の一様分布整数 (int) の分布生成器
std::uniform_int_distribution<uint64_t> get_rand_uni_int( 1, 10 );

//std::random_device rd;
//std::mt19937 mt(rd());

std::vector<std::vector<int>> generateMatrix(int N){
    std::vector<std::vector<int>> matrix(N);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            matrix[i].emplace_back(get_rand_uni_int(mt64));
        }
    }
    return matrix;
}

std::vector<std::vector<int>> multiplyMatrix(std::vector<std::vector<int>> A,std::vector<std::vector<int>> B){
    std::vector<std::vector<int>> C(N);
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            int C_ij = 0;
            for(int k = 0; k < N ; k++){
                C_ij += A[i][k] * B[k][j];
            }
            C[i].emplace_back(C_ij);
        }
    }
    return C;
}
double calcurateAverage(double arr[], int size){
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += arr[i];
    }
    return sum/(double)size;
}
double calcurateVariance(double arr[], int size, double average){
    double sum = 0;
    for(int i = 0; i < size; i++){
        sum += (arr[i]-average)*(arr[i]-average);
    }
    return sqrt(sum/(double)size);
}
int main(){
    std::cin>>N;
    std::vector<std::vector<int>> A(N,std::vector<int>(N)),B(N,std::vector<int>(N)),C(N,std::vector<int>(N));
    clock_t start,end;
    double duration[10];
    for(int loop = 0; loop < 10; loop++){
        start = clock();

        A = generateMatrix( N );
        B = generateMatrix( N );
        
        C = multiplyMatrix( A, B );
        
        end = clock(); 
        duration[loop] += (double)(end - start) / CLOCKS_PER_SEC;

    }
    double duration_average = calcurateAverage(duration,10);
    double duration_variance = calcurateVariance(duration,10, duration_average);
    printf("duration average : %lf duration variance : %lf",duration_average,duration_variance);
    return 0;
}