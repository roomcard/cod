#include <bits/stdc++.h>

using namespace std;

void read_csv(const string &filename, vector<double> &x1, vector<double> &x2)
{
    ifstream file(filename);
    string line;
    getline(file, line); // skip header
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

void linear_regression(const vector<double> &x1, const vector<double> &x2, double &b0, double &b1,
                       double &sum_x1, double &sum_x2, double &sum_x1_x2, double &sum_x1_squared)
{
    int n = x1.size();
    sum_x1 = sum_x2 = sum_x1_x2 = sum_x1_squared = 0;

    for (int i = 0; i < n; ++i)
    {
        sum_x1 += x1[i];
        sum_x2 += x2[i];
        sum_x1_x2 += x1[i] * x2[i];
        sum_x1_squared += x1[i] * x1[i];
    }

    b1 = (n * sum_x1_x2 - sum_x1 * sum_x2) / (n * sum_x1_squared - sum_x1 * sum_x1);
    b0 = (sum_x2 - b1 * sum_x1) / n;
}

vector<double> predict(const vector<double> &x1, double b0, double b1)
{
    vector<double> predictions;
    for (const auto &xi : x1)
    {
        double prediction = b0 + b1 * xi;
        predictions.push_back(round(prediction * 100) / 100); // Round to 2 decimal places
    }
    return predictions;
}

void write_output(const string &filename, double b0, double b1, double sum_x1, double sum_x2,
                  double sum_x1_x2, double sum_x1_squared, const vector<double> &predictions)
{
    ofstream outfile(filename);
    if (outfile.is_open())
    {
        outfile << "Sum of x1,Sum of x2,Sum of x1*x2,Sum of x1^2\n";
        outfile << fixed << setprecision(2) << sum_x1 << "," << sum_x2 << ","
                << sum_x1_x2 << "," << sum_x1_squared << "\n";

        outfile << "Intercept (b0),Slope (b1)\n";
        outfile << fixed << setprecision(2) << b0 << "," << b1 << "\n";

        outfile << "Predicted Values\n";
        for (const auto &val : predictions)
        {
            outfile << fixed << setprecision(2) << val << "\n";
        }
        outfile.close();
        cout << "Output written to " << filename << endl;
    }
    else
    {
        cerr << "Error opening output file!" << endl;
    }
}

int main()
{
    vector<double> x1, x2;
    read_csv("C:\\Users\\Siddhesh Kumbhar\\Desktop\\ass dm\\14.Linear\\input.csv", x1, x2);

    double b0, b1;
    double sum_x1, sum_x2, sum_x1_x2, sum_x1_squared;
    linear_regression(x1, x2, b0, b1, sum_x1, sum_x2, sum_x1_x2, sum_x1_squared);

    vector<double> y_pred = predict(x1, b0, b1);
    cout << "Predicted values: ";
    for (const auto &val : y_pred)
    {
        cout << val << " ";
    }
    cout << endl;

    write_output("C:\\Users\\Siddhesh Kumbhar\\Desktop\\ass dm\\14.Linear\\output.csv", b0, b1,
                 sum_x1, sum_x2, sum_x1_x2, sum_x1_squared, y_pred);

    return 0;
}
