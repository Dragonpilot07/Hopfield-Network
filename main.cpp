#include <iostream>
#include <vector>
#include "hopfield_class.h"
#include "hopfield.cpp"
#include "readNwrite.cpp"
float corruption=0.1;
using namespace std;
int numImages=4;
int main() {
    const int numNeurons = w * h;
    vector<vector<vector<int>>> images(numImages, vector<vector<int>>(HEIGHT, vector<int>(WIDTH)));
    vector<string> imageFiles = {
        "1.pbm"
        ,"2.pbm"
        ,"3.pbm"
        ,"4.pbm"
        //"5.pbm"
    };
    vector<vector<int>> patterns(numImages, vector<int>(numNeurons));

    try {
        // Read PBM files
        for (int i = 0; i < numImages; i++) {
            readPBMFile(imageFiles[i], images[i]);
            cout<<"read"<<endl;
        }

        // Convert 2D image arrays to 1D pattern vectors
        for (int i = 0; i < numImages; i++) {
            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    patterns[i][y * WIDTH + x] = images[i][y][x] ? 1 : -1;
                }
            }
        }
        cout<<"pass 1"<<endl;
        // Train the Hopfield network
        vector<vector<int>> weights = trainHopfield(patterns);
        cout << "Hopfield network trained with the input images." << endl;

        // Test the network with a corrupted version of the first pattern
        vector<int> corruptedPattern = corruptMemory(patterns[3], corruption); // 10% corruption

        // Convert corrupted pattern to 2D image for writing
        vector<vector<int>> corruptedImage(HEIGHT, vector<int>(WIDTH));
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                corruptedImage[y][x] = (corruptedPattern[y * WIDTH + x] == 1) ? 1 : 0;
            }
        }

        // Write the corrupted pattern to a new PBM file
        writePBMFile("output/corrupted_pattern.pbm", corruptedImage);
        cout << "Corrupted pattern written to 'corrupted_pattern.pbm'" << endl;

        // Recall the pattern
        vector<int> recalledPattern = synchronousUpdate(corruptedPattern, weights);

        // Convert recalled pattern back to 2D image for writing
        vector<vector<int>> recalledImage(HEIGHT, vector<int>(WIDTH));
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                recalledImage[y][x] = (recalledPattern[y * WIDTH + x] == 1) ? 1 : 0;
            }
        }

        // Write the recalled pattern to a new PBM file
        writePBMFile("output/recalled_pattern.pbm", recalledImage);
        cout << "Recalled pattern written to 'recalled_pattern.pbm'" << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

