#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath> // Include cmath for the round function

using namespace std;

// Function to read CSV file
void read_csv(const string &filename, vector<double> &x1, vector<double> &x2)
{
    ifstream file(filename);
    string line;
    // Skip the header
    getline(file);
    while (getline(file, line))
    {
        stringstream ss(line);
        string value1, value2;
        if (getline(ss, value1, ',') && getline(ss, value2, ','))
        {
            x1.push_back(stod(value1));
            x2.push_back(stod(value2));
        }
    }
}

// Function for linear regression
void linear_regression(const vector<double> &x1, const vector<double> &x2, double &b0, double &b1)
{
    int n = x1.size();
    double sum_x1 = 0, sum_x2 = 0, sum_x1_x2 = 0, sum_x1_squared = 0;

    for (int i = 0; i < n; ++i)
    {
        sum_x1 += x1[i];
        sum_x2 += x2[i];
        sum_x1_x2 += x1[i] * x2[i];
        sum_x1_squared += x1[i] * x1[i];
    }

    cout << "Sum of x1 variable: " << sum_x1 << endl;
    cout << "Sum of x2 variable: " << fixed << setprecision(2) << sum_x2 << endl;
    cout << "Sum of multiplication of x1*x2: " << fixed << setprecision(2) << sum_x1_x2 << endl;
    cout << "Sum of squares of x1: " << fixed << setprecision(2) << sum_x1_squared << endl;

    b1 = (n * sum_x1_x2 - sum_x1 * sum_x2) / (n * sum_x1_squared - sum_x1 * sum_x1);
    b0 = (sum_x2 - b1 * sum_x1) / n;
}

// Function to predict values
vector<double> predict(const vector<double> &x1, double b0, double b1)
{
    vector<double> predictions;
    for (const auto &xi : x1)
    {
        predictions.push_back(round(b0 + b1 * xi * 100) / 100); // Round to 2 decimal places
    }
    return predictions;
}

int main()
{
    vector<double> x1, x2;
    read_csv("C:\\Users\\Siddhesh Kumbhar\\Desktop\\ass dm\\14.Linear\\input.csv", x1, x2);

    double b0, b1;
    linear_regression(x1, x2, b0, b1);
    cout << "Intercept (b0): " << fixed << setprecision(2) << b0 << endl;
    cout << "Slope (b1): " << fixed << setprecision(2) << b1 << endl;

    vector<double> y_pred = predict(x1, b0, b1);
    cout << "Predicted values: ";
    for (const auto &val : y_pred)
    {
        cout << val << " ";
    }
    cout << endl;

    return 0;
}
