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

pair<vector<int>, int> synchronousUpdate(const vector<int> &state, const vector<vector<int>> &weights) {
    int n = state.size(); // or image_size if that's accessible here
    vector<int> currentState = state;
    int steps = 0;
    const int MAX_STEPS = 100;
    double updateProbability = 0.5;  // Probability of updating each neuron (adjust as needed)

    // Random number generation setup
    unsigned seed = chrono::steady_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_real_distribution<> dis(0.0, 1.0);

    while (steps < MAX_STEPS) {
        vector<int> newState(n);

        // Vector to keep track of which neurons to update
        vector<int> neuronsToUpdate;
        for (int i = 0; i < n; ++i) {
            if (dis(gen) < updateProbability) {
                neuronsToUpdate.push_back(i); //Select to be updated based on probability
            }
        }

        // Update selected neurons synchronously
        for (int i : neuronsToUpdate) {
            int sum = 0;
            for (int j = 0; j < n; ++j) {
                sum += weights[i][j] * currentState[j];
            }
            newState[i] = (sum >= 0) ? 1 : -1;
        }

        // Copy the non-updated neurons values to the new state
        for(int i=0; i<n; ++i){
            bool should_update = false;
            for (int k : neuronsToUpdate) {
                if (k==i){
                    should_update = true;
                    break;
                }
            }
            if(!should_update){
                newState[i] = currentState[i];
            }
        }

        steps++;

        if (newState == currentState) {
            break;  // Convergence reached
        }

        currentState = newState;
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
