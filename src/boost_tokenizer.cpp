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

int main() {
    using namespace std;

    ifstream in_data;
    in_data.open("../data/ex1data1.csv");

    string line;

    float alpha = 0.01;

    vector<float> vec;
    vector<vector<float>> X;
    vector<float> y;

    // Read line by line
    while (getline(in_data, line)) {
        string column;
        stringstream line_stream(line);

        vec.push_back(1.0);
        // Delimit the columns on ','
        while (getline(line_stream, column, ',')) {
            vec.push_back(stof(column));
        }

        // Use the last column for y
        y.push_back(vec.back());

        // Push the rest into X
        vec.pop_back();

        // Push vec into X
        X.push_back(vec);

        vec.clear();
    }

    cout << "----- X -----" << endl;
    ;
    for (auto i : X) {
        for (auto j : i) {
            cout << j << " ";
        }
        cout << endl;
    }

    cout << "----- y -----" << endl;
    for (auto i : y) {
        cout << i << endl;
    }

    /* Number of samples */
    uint_fast16_t m = X.size();
    /* Number of features */
    uint_fast16_t n = X[0].size();

    cout << "Number of samples (m): " << m << endl;
    cout << "Number of features (n): " << n - 1 << endl;

    /* Initialize theta */
    vector<float> theta(n);
    for (auto &t : theta) {
        t = 1;
    }

    cout << "theta" << endl;
    for (auto &k : theta) {
        cout << k << endl;
    }

    {
        /* Calculate h_theta(x) */
        vector<float> theta_transpose_x(m);
        for (uint_fast16_t i = 0; i < m; i++) {
            vector<float> &x_i = X[i];
            float pp = 0;
            for (uint_fast16_t j = 0; j < n; j++) {
                pp = pp + (x_i[j] * theta[j]);
            }
            theta_transpose_x[i] = pp;
        }

        cout << "theta_transpose_x" << endl;
        for (auto ttx : theta_transpose_x) {
            cout << ttx << endl;
        }

        /* Subtract y from theta_transpose_x */
        vector<float> theta_transpose_x_minus_y(m);
        for (uint_fast16_t i = 0; i < m; i++) {
            theta_transpose_x_minus_y[i] = theta_transpose_x[i] - y[i];
        }

        cout << "theta_transpose_x_minus_y" << endl;
        for (auto htxy : theta_transpose_x_minus_y) {
            cout << htxy << endl;
        }

        float foo = 0;
        for (auto &i : theta_transpose_x_minus_y) {
            foo = foo + (i * i);
        }
        foo = foo / m;
        cout << "MSE: " << foo << endl;
    }

    /* Calculate new theta */
    vector<float> new_theta(n);
    for (uint_fast16_t i = 0; i < n; i++) {
        float pp = 0;
        for (uint_fast16_t j = 0; j < m; j++) {
            pp = pp + (theta_transpose_x_minus_y[j] * X[j][i]);
        }
        new_theta[i] = pp;
    }

    float bar = alpha / m;
    for (auto &i : new_theta) {
        i = bar * i;
    }

    cout << "new_theta" << endl;
    for (auto &k : new_theta) {
        cout << k << endl;
    }

    // Now calculate theta_transpose_x - y to calculate error
    vector<float> new_theta_transpose_x(m);
    for (uint_fast16_t i = 0; i < m; i++) {
        vector<float> &x_i = X[i];
        float pp = 0;
        for (uint_fast16_t j = 0; j < n; j++) {
            pp = pp + (x_i[j] * new_theta[j]);
        }
        new_theta_transpose_x[i] = pp;
    }

    cout << "new_theta_transpose_x" << endl;
    for (auto ttx : new_theta_transpose_x) {
        cout << ttx << endl;
    }

    /* Subtract y from h_theta(x) */
    vector<float> new_h_theta_x_minus_y(m);
    for (uint_fast16_t i = 0; i < m; i++) {
        new_h_theta_x_minus_y[i] = new_theta_transpose_x[i] - y[i];
    }

    foo = 0;
    for (auto &i : new_h_theta_x_minus_y) {
        foo = foo + (i * i);
    }
    foo = foo / m;
    cout << "MSE: " << foo << endl;
}