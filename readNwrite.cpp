    #include <iostream>
    #include <fstream>
    #include <string>
    #include <sstream>
    #include <stdexcept>

    using namespace std;

    const int WIDTH = 16;
    const int HEIGHT = 16;

    void readPBMFile(const string& filename, vector<vector<int>>& image) {
        ifstream file(filename);
        string line;

        
        getline(file, line);
        if (line != "P1") {
            throw runtime_error("Not a valid P1 PBM file");
        }

        // Skip comments
        while (getline(file, line) && line[0] == '#') {}

        // Read dimensions (assuming 16x16)
        int width, height;
        stringstream(line) >> width >> height;
        if (width != WIDTH || height != HEIGHT) {
            throw runtime_error("Image dimensions do not match 16x16");
        }

        // Read pixel data
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                file >> image[y][x];
            }
        }
    }

    void writePBMFile(const string& filename, vector<vector<int>>& image) {
        ofstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Unable to open file for writing");
        }
    
        // Write header
        file << "P1" << endl;
        file << "# Created by PBM Writer" << endl;
        file << WIDTH << " " << HEIGHT << endl;
    
        // Write pixel data
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                file << (image[y][x] ? "1" : "0");
                if (x < WIDTH - 1) file << " ";
            }
            file << endl;
        }
    
        file.close();
        cout << "PBM file written successfully." << endl;
    }

    /*int main() {
        int image[HEIGHT][WIDTH];
        try {
            readPBMFile("C:\\Users\\user\\Desktop\\neuro projekt\\example.pbm", image);
            
            // Print the image (optional)
            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    cout << (image[y][x] ? '#' : ' ');
                }
                cout << endl;
            }
            writePBMFile("test.pbm", image);

        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            return 1;
        }
        return 0;
    }*/
