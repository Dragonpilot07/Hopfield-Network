#include "hopfield_class.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
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


std::mt19937& getGenerator() {
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

vector<int> corruptMemory(const vector<int>& memory, double p) {
    // Use epoch time (in nanoseconds) as the seed
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    bernoulli_distribution dist(p);

    vector<int> corrupted = memory;
    for (size_t i = 0; i < memory.size(); ++i) {
        if (dist(generator)) {
            corrupted[i] *= -1;
        }
    }
    return corrupted;
}

pair<vector<int>, int> synchronousUpdate(const vector<int>& state, 
    const vector<vector<int>>& weights) {
const int n = state.size();
vector<int> currentState = state;
int steps = 0;
const int MAX_STEPS = 1000;
const float updateProbability = 0.5f;

random_device rd;
mt19937 gen(rd());
bernoulli_distribution bern(updateProbability);
vector<int> indices(n);
iota(indices.begin(), indices.end(), 0);

while (steps < MAX_STEPS) {
vector<int> newState = currentState;
bool updated = false;

// Shuffle indices for true randomness
shuffle(indices.begin(), indices.end(), gen);

for (int i : indices) {
if (bern(gen)) {
int sum = inner_product(weights[i].begin(), weights[i].end(),
  currentState.begin(), 0);
int newValue = (sum >= 0) ? 1 : -1;

if (newValue != newState[i]) {
newState[i] = newValue;
steps++;
updated = true;
}
}
}

if (!updated) break;
currentState = move(newState);
}

return {currentState, steps};
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

void normalizeWeights(int n,vector<vector<int>> weights){
    for(int i=0;i<16;i++){
        for(int j=0;j<16;j++){
            weights[i][j]/=4;
        }
    }
}
vector<int> cropMemory(const vector<int>& memory, int crop_size) {
    if (crop_size >= w || crop_size <= 0) {
        cerr << "Invalid crop size.  Must be between 0 and " << WIDTH << endl;
        return memory; // Or handle the error as appropriate
    }

    vector<int> cropped_memory = memory;
    int start_row = (h - crop_size) / 2;
    int start_col = (w - crop_size) / 2;

    for (int row = 0; row < h; ++row) {
        for (int col = 0; col < w; ++col) {
            if (row < start_row || row >= start_row + crop_size ||
                col < start_col || col >= start_col + crop_size) {
                cropped_memory[row * w + col] = -1;  // Set to "black" (-1)
            }
        }
    }
    return cropped_memory;
}
