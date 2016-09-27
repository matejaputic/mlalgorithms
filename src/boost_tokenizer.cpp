/*
    This version of the tokenizer parses floats.
    The first <N-1> columns are parsed into X
    The last column is parsed into Y

*/

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
    vector<vector<float> > X;
    vector<float> y;

    // Read line by line
    while (getline(in_data, line))
    {
        string column;
        stringstream line_stream(line);

        // Delimit the columns on ','
        while (getline(line_stream, column, ',')) {
            vec.push_back(stof(column));
        }

        // Use the last column for y
        y.push_back(vec.back());

        // Push the rest into X
        vec.pop_back();
        X.push_back(vec);
        
        vec.clear();
    }

    cout << "----- X -----" << endl;;
    for(auto i: X) {
        for(auto j: i){
            cout << j << " ";
        }
        cout << endl;
    }

    cout << "----- y -----" << endl;
    for(auto i: y){
        cout << i << endl;
    }
}