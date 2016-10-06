/*
    This version of the tokenizer parses floats.
    The first <N-1> columns are parsed into X
    The last column is parsed into Y

*/

#include <iostream> // cout, endl
#include <fstream>  // fstream
#include <sstream>  // sstream
#include <vector>
#include <string>
#include <algorithm> // copy
#include <iterator>  // ostream_operator
#include <cstdbool>

#define DEBUG false

using namespace std;

void readcsv(ifstream &data_stream, vector<vector<float>> &X,
             vector<float> &y) {

    string line;
    vector<float> vec;

    while (getline(data_stream, line)) {
        string column;
        stringstream line_stream(line);

        vec.push_back(1.0);

        while (getline(line_stream, column, ',')) {
            vec.push_back(stof(column));
        }

        y.push_back(vec.back());
        vec.pop_back();
        X.push_back(vec);
        vec.clear();
    }
}

/*

    Very inefficient way to transpose X to X_t

*/
void transpose(vector<vector<float>> &X, vector<vector<float>> &X_t) {

    // Transpose X
    for (uint_fast16_t i = 0; i < X[0].size(); i++) {

        vector<float> temp;

        for (uint_fast16_t j = 0; j < X.size(); j++) {
            temp.push_back(X[j][i]);
        }
        X_t.push_back(temp);
    }
}

/*
    Function to find the inner product of matrix A[m][n] and vector b[n]

*/

/**
 * This function computes a dot product between A and b
 * In row major ordering, takes an MxN matrix A and a Nx1 vector b
 * @param A      A matrix of floats
 * @param b      A vector of floats
 * @param result [description]
 */
void A_dot_b(vector<vector<float>> &A, vector<float> &b,
             vector<float> &result) {

    // Verify dimensions
    if (A[0].size() != b.size()) {
        cout << "**SIZE MISMATCH (A[0].size(), b.size()): " << A[0].size()
             << "," << b.size() << endl;
        exit(0);
    }

    // For each datapoint
    for (uint_fast16_t i = 0; i < A.size(); i++) {
        // Grab all of the current row's X values
        vector<float> x_i = A[i];

        float pp = 0;

        // Dot product (inner-product) of the current column and row
        for (uint_fast16_t j = 0; j < b.size(); j++) {
            if (DEBUG)
                cout << "A_dot_b: i: " << i << " j: " << j << endl;
            if (DEBUG)
                cout << "A_dot_b: " << x_i[j] << " times " << b[j] << endl;
            pp += (x_i[j] * b[j]);
        }
        if (DEBUG)
            cout << "A_dot_b: result[" << i << "]: " << pp << endl;
        result[i] = pp;
    }
}

int main(int argc, char *argv[]) {

    uint_fast16_t num_epochs = 1000;

    if (argc == 2)
        num_epochs = stoi(argv[1]);

    cout << "Num epochs: " << num_epochs << endl;

    ifstream in_data;
    in_data.open("../data/ex1data2.csv");

    float alpha = 0.0000000001;

    vector<vector<float>> X;
    vector<float> y;

    readcsv(in_data, X, y);

    // Let's transpose X
    vector<vector<float>> X_t;
    transpose(X, X_t);

    /* Number of samples */
    uint_fast16_t m = X.size();

    /* Number of features */
    uint_fast16_t n = X[0].size();

    cout << "Number of samples (m): " << m << endl;
    cout << "Number of features (n-1): " << n - 1 << endl;
    cout << "Dimension of X: " << X.size() << ", " << X[0].size() << endl;
    cout << "Dimensions of X_t: " << X_t.size() << ", " << X_t[0].size()
         << endl;
    cout << "------------------------" << endl;

    /* Initialize theta to ones (Ideally we want this to be 'guesses') */
    vector<float> theta(n);
    for (auto &t : theta) {
        t = 1.0;
    }

    vector<float> X_theta(m);
    /* The error function (J(Theta)) */
    vector<float> error(m);
    /* Gradient function (*/
    vector<float> gradient(n);

    for (uint_fast16_t epoch = 0; epoch < num_epochs; epoch++) {

        cout << "Iter: " << epoch << endl;

        /* Calculate hypothesis */
        A_dot_b(X, theta, X_theta);
        if (DEBUG) {
            cout << "main: X_theta[]:" << endl;
            for (auto &x : X_theta)
                cout << "    " << x << endl;
        }

        /* Find distance between hypothesis and each x */
        for (uint_fast16_t i = 0; i < X.size(); i++) {
            error[i] = X_theta[i] - y[i];
        }
        if (DEBUG) {
            cout << "main: error[]:" << endl;
            for (auto &x : error)
                cout << "    " << x << endl;
        }

        /* Calculate MSE */
        float MSE = 0.0;
        for (auto &i : error)
            MSE += (i * i);
        MSE /= error.size();

        /* Calculate the gradient vector
         * In this case it's just the sum of the errors in each direction */
        A_dot_b(X_t, error, gradient);
        if (DEBUG) {
            cout << "main: gradient[]:" << endl;
            for (auto &x : gradient)
                cout << "    " << x << endl;
        }

        /* Update the theta */
        for (uint_fast16_t i = 0; i < n; i++) {
            theta[i] = theta[i] - (alpha)*gradient[i];
        }
        if (DEBUG) {
            cout << "main: theta[]:" << endl;
            for (auto &x : theta)
                cout << "    " << x << endl;
        }

        cout << "MSE: " << MSE << endl;
        cout << "THETA: (";
        for (auto k : theta) {
            cout << k << " ";
        }
        cout << ")\n--" << endl;
    }
}
