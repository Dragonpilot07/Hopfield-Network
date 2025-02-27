#include <iostream>
#include <vector>
#include <string>
#include "hopfield.cpp"
#include "readNwrite.cpp"  // Make sure this is correct and contains implementations

float corruption = 0.1;

using namespace std;

int main() {
    const int numNeurons = w * h;
    int numImages = 4;

    vector<string> imageFiles = {
        "1.pbm", "2.pbm", "3.pbm", "4.pbm"
    };

    vector<vector<vector<int>>> images(numImages, vector<vector<int>>(h, vector<int>(w)));
    vector<vector<int>> patterns(numImages, vector<int>(numNeurons));

    try {
        // Read PBM files and convert to 1D patterns
        for (int i = 0; i < numImages; i++) {
            readPBMFile(imageFiles[i], images[i]);

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    patterns[i][y * w + x] = (images[i][y][x] == 1) ? 1 : -1; // Convert pixel values to 1 and -1
                }
            }
            cout << "Read " << imageFiles[i] << endl;
        }

        // Train the Hopfield network
        vector<vector<int>> weights = trainHopfield(patterns);

        // Normalize the weights (IMPORTANT!)
        normalizeWeights(numImages, weights);
        cout << "Hopfield network trained and weights normalized." << endl;

        // Test with a corrupted version of one pattern
        vector<int> corruptedPattern = corruptMemory(patterns[3], corruption);

        // Convert corrupted pattern to 2D image for writing
        vector<vector<int>> corruptedImage(h, vector<int>(w));
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                corruptedImage[y][x] = (corruptedPattern[y * w + x] == 1) ? 1 : 0;
            }
        }
        writePBMFile("output/corrupted_pattern.pbm", corruptedImage);
        cout << "Corrupted pattern written to 'output/corrupted_pattern.pbm'" << endl;

        // ------ SYNCHRONOUS UPDATE ------
        pair<vector<int>, int> syncResult = synchronousUpdate(corruptedPattern, weights);
        vector<int> recalledPatternSync = syncResult.first;
        int syncSteps = syncResult.second;

        vector<vector<int>> recalledImageSync(h, vector<int>(w));
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                recalledImageSync[y][x] = (recalledPatternSync[y * w + x] == 1) ? 1 : 0;
            }
        }
        writePBMFile("output/recalled_pattern_sync.pbm", recalledImageSync);
        cout << "Recalled pattern (synchronous) written to 'output/recalled_pattern_sync.pbm' after " << syncSteps << " steps." << endl;

        // ------ ASYNCHRONOUS UPDATE ------
        vector<int> recalledPatternAsync = asynchronousUpdate(corruptedPattern, weights);

        vector<vector<int>> recalledImageAsync(h, vector<int>(w));
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                recalledImageAsync[y][x] = (recalledPatternAsync[y * w + x] == 1) ? 1 : 0;
            }
        }
        writePBMFile("output/recalled_pattern_async.pbm", recalledImageAsync);
        cout << "Recalled pattern (asynchronous) written to 'output/recalled_pattern_async.pbm'" << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
