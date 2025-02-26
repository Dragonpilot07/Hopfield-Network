
#ifndef HOPFIELD_CLASS_H
#define HOPFIELD_CLASS_H
#include <iostream>
#include <vector>
using namespace std;
const int w=16;
const int h=16;
const int image_size=h*w;
vector<vector<int>> trainHopfield(const vector<vector<int>> &memories);
vector<int> corruptMemory(const vector<int> &memory,double p);
vector<int> synchronousUpdate(const vector<int> &state,const vector<vector<int>> &weights);
vector<int> asynchronousUpdate(vector<int> state, const vector<vector<int>> &weights);
vector<int> cropMemory(const vector<int> &memory,int crop_size);
void normalizeWeights(vector<vector<int>>& weights, int numPatterns);
#endif
