#include <iostream>
#include <vector>
#include "hopfield_class.h"
#include "hopfield.cpp"
#include "readNwrite.cpp"

using namespace std;

int main() {
    const int numNeurons = w * h;
    int image[HEIGHT][WIDTH];
    int image2[HEIGHT][WIDTH];
    vector<int> pattern(numNeurons);
    vector<int> pattern2(numNeurons);

    try {
        // Read the PBM file
        readPBMFile("C:\\Users\\user\\Desktop\\neuro projekt\\example.pbm", image);
        readPBMFile("C:\\Users\\user\\Desktop\\neuro projekt\\2.pbm", image2);

        // Convert 2D image array to 1D pattern vector
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                pattern[y * WIDTH + x] = image[y][x] ? 1 : -1;
            }
        }
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                pattern2[y * WIDTH + x] = image2[y][x] ? 1 : -1;
            }
        }

        // Create a vector of memories (in this case, just one pattern)
        vector<vector<int>> memories = {pattern2,pattern};

        // Train the Hopfield network
        vector<vector<int>> weights = trainHopfield(memories);

        cout << "Hopfield network trained with the input image." << endl;

        // Optional: Test the network with a corrupted version of the pattern
        vector<int> corruptedPattern = corruptMemory(pattern2, 0.1); // 10% corruption
        
        int corruptedImage[HEIGHT][WIDTH];
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                corruptedImage[y][x] = (corruptedPattern[y * WIDTH + x] == 1) ? 1 : 0;
            }
        }

        // Write the corrupted pattern to a new PBM file
        writePBMFile("corrupted_pattern.pbm", corruptedImage);
        cout << "Corrupted pattern written to 'corrupted_pattern.pbm'" << endl;

        vector<int> recalledPattern = synchronousUpdate(corruptedPattern, weights);

        // Convert recalled pattern back to 2D image for writing
        int recalledImage[HEIGHT][WIDTH];
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                recalledImage[y][x] = (recalledPattern[y * WIDTH + x] == 1) ? 1 : 0;
            }
        }

        // Write the recalled pattern to a new PBM file
        writePBMFile("recalled_pattern.pbm", recalledImage);
        cout << "Recalled pattern written to 'recalled_pattern.pbm'" << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
