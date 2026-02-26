#include "eigen/Eigen/Dense"
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

int main(void)
{
    unsigned int word_count = 0;
    const string delimiters = ".?-,!: ";
    string myLine;
    ifstream INPUT_FILE("test.txt");
    unordered_map<string, int> Text_Unordered_Map;
    unsigned int unordered_map_index = 0;
    while (getline(INPUT_FILE, myLine))
    {
        // 1.Read the word
        // 2. Save the word and just print it for now.
        unsigned int line_index = 0;
        while (line_index < myLine.length())
        {
            string word = "";
            while (delimiters.find(myLine[line_index]) == string::npos && line_index < myLine.length())
            {
                myLine[line_index] = tolower(myLine[line_index]);
                word += myLine[line_index];
                line_index++;
            }
            while (delimiters.find(myLine[line_index]) != string::npos && line_index < myLine.length())
                line_index++;
            // Add word to unordered map.If the word is not there add it
            if (Text_Unordered_Map.find(word) == Text_Unordered_Map.end())
            {
                Text_Unordered_Map[word] = unordered_map_index;
                unordered_map_index++;
            }
            word_count++;
            // cout << word << "|";
            /*
            DEBUGGING:
              for (auto &pair : Text_Unordered_Map)
            {
                cout << pair.first << " -> " << pair.second << "\n";
            }
            */
        }
    }
    // Now it is time to use the map to get the list:
    cout << "\nMAP SIZE: " << Text_Unordered_Map.size() << " INDEX:" << unordered_map_index << endl;
    // Initialize the matrix
    unsigned int MAP_SIZE = Text_Unordered_Map.size();
    vector<vector<int>> A(MAP_SIZE, vector<int>(MAP_SIZE, 0));
    // Now go through the matrix and assign its elements

    // GO BACK TO THE BEGINNING OF THE FILE
    if (INPUT_FILE.eof())
    {
        INPUT_FILE.clear();
    }
    INPUT_FILE.seekg(0, std::ios::beg);
    string prev_word_1, prev_word_2;
    // For this simple example we will just copy future versions will be far more productive
    while (getline(INPUT_FILE, myLine))
    {
        // 1.Read the word
        // 2. Save the word and just print it for now.
        unsigned int line_index = 0;
        while (line_index < myLine.length())
        {
            string word = "";
            while (delimiters.find(myLine[line_index]) == string::npos && line_index < myLine.length())
            {
                myLine[line_index] = tolower(myLine[line_index]);
                word += myLine[line_index];
                line_index++;
            }

            while (delimiters.find(myLine[line_index]) != string::npos && line_index < myLine.length())
                line_index++;
            if (unordered_map_index > 1)
            {
                // 1.Get map value
                // 2.Increase the value of word.
                int next_next_word_index = Text_Unordered_Map[word];
                int next_word_index = Text_Unordered_Map[prev_word_1];
                int current_word_index = Text_Unordered_Map[prev_word_2];
                A[current_word_index][next_next_word_index]++;
                A[next_next_word_index][current_word_index]++;
                A[current_word_index][next_word_index] += 2;
                A[next_word_index][current_word_index] += 2;
            }

            if (unordered_map_index > 1)
                prev_word_2 = prev_word_1;
            prev_word_1 = word;
        }
    }
    /*// Now lets print A.
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
            cout << " " << A[i][j] << " ";
        cout << endl;
    }
    */
    // Lets create the laplacian
    vector<vector<int>> L(MAP_SIZE, vector<int>(MAP_SIZE, 0));

    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        int degree = 0;
        for (size_t j = 0; j < MAP_SIZE; j++)
        {
            degree += A[i][j];
            L[i][j] = -A[i][j];
        }
        L[i][i] = degree;
    }

    /*// Now lets print Laplacian.
    for (size_t i = 0; i < MAP_SIZE; i++)
    {
        for (size_t j = 0; j < MAP_SIZE; j++)
            cout << " " << L[i][j] << " ";
        cout << endl;
    }
    */
    // Now LEts find the fidler value.
    Eigen::MatrixXd M(MAP_SIZE, MAP_SIZE);
    for (size_t i = 0; i < MAP_SIZE; i++)
        for (size_t j = 0; j < MAP_SIZE; j++)
            M(i, j) = L[i][j];

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(M);
    double fiedler = solver.eigenvalues()(1);
    double normalized_fiedler = fiedler / (double)word_count;
    cout << "Normalized Fiedler value: " << normalized_fiedler << endl;
    cout << "Fiedler Value: " << fiedler << endl;
    return 0;
}
