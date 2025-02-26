#include "hopfield_class.h"
#include <iostream>
#include <vector>
#include <random> 
#include <chrono>
static int counter=0;
using namespace std;
vector<vector<int>> trainHopfield(const vector<vector<int>> &memories){
    int n=image_size;
    vector<vector<int>> weights(n,vector<int>(n,0));
    for(const auto &memory:memories){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(i!=j){
                    weights[i][j]+=memory[i]*memory[j];
                }
            }
        }
    }
    return weights;
}
#include <random>

std::mt19937& getGenerator() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

vector<int> corruptMemory(const vector<int>& memory, double p) {
    std::bernoulli_distribution dist(p);
    vector<int> corrupted = memory;
    for (size_t i = 0; i < memory.size(); ++i) {
        if (dist(getGenerator())) {
            corrupted[i] *= -1;
        }
    }
    return corrupted;
}

vector<int> synchronousUpdate(const vector<int> &state,const vector<vector<int>> &weights){
    int n=image_size;
    vector<int> newState(n);
    for(int i=0;i<n;i++){
        int sum=0;
        for(int j=0;j<n;j++){
            sum+=weights[i][j]*state[j];
        }
        newState[i]=(sum>=0)? 1:-1;
        counter++;
    }
    return newState;
}
vector<int> asynchronousUpdate(vector<int> state,const vector<vector<int>> &weights){
    int n=image_size;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0,n-1);
    for(int k=0;k<n;k++){
        int i=dist(gen);
        int sum=0;
        for(int j=0;j<n;j++){
            sum+=weights[i][j]*state[j];
        }
        state[i]=(sum>=0)?1:-1;
    }
    return state;
}


vector<int> corruptMemory(const vector<int> &memory, double p) {
    static mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    bernoulli_distribution dist(p);

    vector<int> corrupted = memory;
    for (int i = 0; i < image_size; i++) {
        if (dist(gen)) {
            corrupted[i] *= -1;
        }
    }
    return corrupted;
}

void normalizeWeights(int n,vector<vector<int>> weights){
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            weights[i][j]/=4;
        }
    }
}
