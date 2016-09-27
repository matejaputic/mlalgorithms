#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <sstream>      // sstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

int main()
{
    using namespace std;

    ifstream in_data;
    in_data.open("../data/ex1data1.csv");

    string line;

    vector< float > vec;
    vector<vector<float>> X;
    vector<float> y;

    while (getline(in_data, line))
    {
        string column;
        stringstream line_stream(line);
        while (getline(line_stream, column, ',')) {
            vec.push_back(stof(column));
        }
        X.push_back(vec);
        vec.clear();
    }

    for(auto i: X) {
        for(auto j: i){
            cout << j << " ";
        }
        cout << endl;
    }
}