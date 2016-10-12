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
#include <algorithm> // copy, count
#include <iterator>  // ostream_operator
#include <cstdbool>  // true and false
#include <Eigen/Dense>
#include "getopt.h" // Option parser

using namespace std;
using namespace Eigen;

/*
  Print the usage of the program
*/
void usage(char * argv) {

  printf("USAGE: %s [OPTIONS] <input filename> \n", argv);
  printf("  -e               The number of iterations (epochs) to be computed\n");
  printf("  -a               The step size for each update\n");

  printf("\n MISC:\n");
  printf("  -h               Print this help and exit\n");
  printf("\n");
}

void getCSVNumRowsCols(ifstream &data_stream, uint_fast16_t &rows,
  uint_fast16_t &cols) {
    string line;
    getline(data_stream, line);
    cols = count(line.begin(), line.end(), ',') + 1;
    rows = 1;
    while (getline(data_stream, line)) {
      rows++;
    }

    // Clear the failbit if set, and rewind to beginning
    data_stream.clear();
    data_stream.seekg(0);
  }

  /**
  * Assumes row-major indexing
  * Assumes X is size (rows, cols-1)
  * Assumes y is size (rows, 1)
  * Assumes data is of type float
  * Assumes there are at least two columns and one row in the ifstream
  * Assumes delimiter is ','
  * Assigns first 1.0 to first column of X
  * @param data_stream [description]
  * @param rows        [description]
  * @param cols        [description]
  * @param X           [description]
  * @param y           [description]
  */
  void readcsv(ifstream &data_stream, uint_fast16_t rows, uint_fast16_t cols,
    MatrixXf &X, VectorXf &y) {
      string line;
      string column;

      uint_fast16_t row_idx = 0;
      uint_fast16_t X_col_idx = 0;

      // Copy the rest of the rows
      while (row_idx < rows) {
        /* Preamble */
        X_col_idx = 0;
        X(row_idx, X_col_idx++) = 1.0;
        // X_col_idx++;

        getline(data_stream, line);
        string::size_type pos = 0;
        X(row_idx, X_col_idx++) = stof(line.substr(pos));
        // X_col_idx++;
        while (X_col_idx <= cols) {
          pos = line.find(',', pos);
          if (pos == string::npos) {
            // Reached the end of the row
            break;
          }
          pos++;
          if (X_col_idx == cols) {
            // Reached the end of X, add to y
            y(row_idx) = stof(line.substr(pos));
          } else {
            X(row_idx, X_col_idx++) = stof(line.substr(pos));
          }
        }
        row_idx++;
      }

      // while (getline(line_stream, column, ',')) {
      //     X(row_idx, col_idx++) = stof(column);
      // }

      // Continue copying rest of rows
      // while (basic_istream::getline(data_stream, line)) {
      //     string column;
      //     stringstream line_stream(line);

      //     vec.push_back(1.0);

      //     while (basic_istream::getline(line_stream, column, ',')) {
      //         X(row, col++) = stof(column);
      //         vec.push_back(stof(column));
      //     }

      //     y.push_back(vec.back());
      //     vec.pop_back();
      //     X.push_back(vec);
      //     vec.clear();
      // }
    }

    /*

    Very inefficient way to transpose X to X_t

    */
    void transpose(vector<vector<float> > &X, vector<vector<float> > &X_t) {

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
    void A_dot_b(vector<vector<float> > &A, vector<float> &b,
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
            pp += (x_i[j] * b[j]);
          }
          result[i] = pp;
        }
      }

      int main(int argc, char *argv[]) {

        uint_fast16_t num_epochs = 10;  // Default number of epochs
        float alpha = 0.000000003;      // Default alpha
        int c;
        opterr = 0;
        bool DEBUG = false;


        // We're looking for an epoch, alpha, and/or help
        const char * short_opt = "de:a:h";

        // Parse command-line arguments
        while((c = getopt(argc, argv, short_opt)) != -1) {
          switch(c)
          {
            case 'e':
              num_epochs = atoi(optarg);
            break;

            case 'a':
              alpha = atof(optarg);
            break;

            case 'd':
              DEBUG = true;
            break;

            case 'h':
              usage(argv[0]);
            return(0);

            default:
              fprintf(stderr, "%s: invalid option -- %c\n", argv[0], c);
              usage(argv[0]);
            return(-2);
          };
        };

        if(optind >= argc) {
          usage(argv[0]);
          exit(EXIT_FAILURE);
        }

        // Grab the filename from argv
        string fn(argv[optind++]);

        // Open the file
        ifstream in_data;
        in_data.open(fn);

        if(!in_data.good())
        {
          std::cout << "ERROR: Could not read file: " << fn << std::endl;
          return(-1);
        }

        uint_fast16_t rows, cols;

        getCSVNumRowsCols(in_data, rows, cols);

        MatrixXf X(rows, cols);
        VectorXf y(rows);

        if (DEBUG)
          cout << "Rows, cols: (" << rows << ", " << cols << ")" << endl;

        readcsv(in_data, rows, cols, X, y);

        in_data.close();

        // Transpose X
        MatrixXf X_t(cols, rows);
        X_t = X.transpose();

        /* Number of samples */
        uint_fast16_t m = rows;

        /* Number of features */
        uint_fast16_t n = cols;

        if (DEBUG) {
          cout << "Number of samples (m): " << m << endl;
          cout << "Number of features (n-1): " << n - 1 << endl;
          cout << "------------------------" << endl;
        }

        // Initialize theta to ones (Ideally we want this to be 'guesses')
        VectorXf theta(n);
        /* Set guesses to all 1s */
        theta.setOnes();

        /* Hypothesis */
        VectorXf X_theta(m);
        /* The error function (J(Theta)) */
        VectorXf error(m);
        /* Gradient function */
        VectorXf gradient(n);

        for (uint_fast16_t epoch = 0; epoch < num_epochs; epoch++) {
          if (DEBUG)
            cout << "**** Iter: " << epoch << " ****" << endl;

          /* Calculate hypothesis */
          X_theta = X * theta;

          if (DEBUG)
            cout << "X_theta: " << endl << X_theta << endl;

          /* Find distance between hypothesis and each x */
          error = X_theta - y;

          if (DEBUG)
            cout << "MSE: " << error.squaredNorm() / m << endl;

          if (DEBUG)
            cout << "error: " << endl << error << endl;

          /* Calculate the gradient vector
          * In this case it's just the sum of the errors in each direction */
          gradient = X_t * error;

          if (DEBUG)
            cout << "gradient: " << endl << gradient << endl;

          /* Update the theta */
          theta = theta - (alpha * gradient);

          if (DEBUG)
            cout << "theta: " << endl << theta << endl;
        }

        cout << "MSE: " << error.squaredNorm() / m << endl;
        cout << "THETA: " << theta << endl;
      }
