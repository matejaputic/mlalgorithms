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

void readcsv(std::ifstream& data_stream, std::vector< std::vector<float> > &X, std::vector<float> &y)
{
    using namespace std;

    string line;
    vector<float> vec;

    while(getline(data_stream, line))
    {
        string column;
        stringstream line_stream(line);

        vec.push_back(1.0);

        while(getline(line_stream, column, ','))
        {
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
void transpose(std::vector<std::vector<float> > &X, std::vector<std::vector<float> > &X_t)
{
    using namespace std;

    // Transpose X
    for(uint_fast16_t i = 0; i < X[0].size(); i++){

        vector<float> temp;

        for(uint_fast16_t j = 0; j < X.size(); j++)
        {
            temp.push_back(X[j][i]);
        }
        X_t.push_back(temp);
    }
}

/*
    Function to find the inner product of matrix A[m][n] and vector b[n]
*/
void A_dot_b(std::vector<std::vector<float> > &A, std::vector<float> &b, std::vector<float> &result)
{
    using namespace std;

    // Verify dimensions
    if(A[0].size() != b.size())
    {
        cout << "**SIZE MISMATCH (A[0].size(), b.size()): " << A[0].size() << "," << b.size() << endl;
        exit(0);
    }

    // For each datapoint
    for(uint_fast16_t i = 0; i < A.size(); i++)
    {
        // Grab all of the current-dimension's X values
        vector<float> x_i = A[i];

        float pp = 0;

        // Dot product (inner-product) of the current column and row
        for(uint_fast16_t j = 0; j < b.size(); j++)
        {
            pp += (x_i[j] * b[j]);

        }
        result[i] = pp;
    }
}

int main(int argc, char *argv[]) {
    using namespace std;

    uint_fast16_t num_epochs = 1000;

    if(argc == 2)
        num_epochs = stoi(argv[1]);

    cout << "Num epochs: " << num_epochs << endl;

    ifstream in_data;
    in_data.open("../data/ex1data1.csv");

    float alpha = 0.0001;

    vector<vector<float> > X;
    vector<float> y;

    readcsv(in_data, X, y);

    // Let's transpose X
    vector<vector<float> > X_t;
    transpose(X, X_t);

    /* Number of samples */
    uint_fast16_t m = X.size();

    /* Number of features */
    uint_fast16_t n = X[0].size();

    cout << "Number of samples (m): " << m << endl;
    cout << "Number of features (n-1): " << n - 1 << endl;
    cout << "Dimension of X: " << X.size() << ", " << X[0].size() << endl;
    cout << "Dimensions of X_t: " << X_t.size() << ", " << X_t[0].size() << endl;
    cout << "------------------------" << endl;

    /* Initialize theta to ones (Ideally we want this to be 'guesses') */
    vector<float> theta(n);
    for (auto &t : theta) {
        t = 1.0;
    }

    vector<float> X_theta(m);
    vector<float> error(m);
    vector<float> gradient(n);

    for(uint_fast16_t epoch = 0; epoch < num_epochs; epoch++)
    {

        A_dot_b(X, theta, X_theta);

        /* Subtract y from theta_transpose_x */
        for (uint_fast16_t i = 0; i < X.size(); i++) {
            error[i] = X_theta[i] - y[i];
        }

        // Calculate MSE
        long MSE = 0.0;
        for (auto &i : error)
            MSE += (i * i);
        MSE /= error.size();

        /* Calculate the gradient vector */
        A_dot_b(X_t, error, gradient);

        // Update the theta
        for(uint_fast16_t i = 0; i < n; i++)
        {
            theta[i] = theta[i] - (alpha) * gradient[i];
            cout << "Gradient: " << gradient[i] << endl;
        }

        cout << "MSE: " << MSE << endl;
        cout << "THETA: (";
        for (auto k : theta) {
            cout << k << " ";
        }
        cout << ")\n--" << endl;

    }
}
