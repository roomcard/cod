#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>

double calculateCorrelation(const std::string &file_path, int &n)
{
    // Lists to store data
    std::vector<double> x_values;
    std::vector<double> y_values;

    // Read data from CSV
    std::ifstream file(file_path);
    std::string line;
    // Skip the header
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        double x, y;
        std::string comma;
        // ss >> x >> comma >> y; // Read x and y, expecting a comma in between
        getline(ss, comma, ','); // upto the comma in the line
        x = std::stod(comma);    // convert the string to double
        getline(ss, comma, ','); // upto the comma in the line
        y = std::stod(comma);    // convert the string to double
        x_values.push_back(x);
        y_values.push_back(y);
    }

    n = x_values.size(); // Number of pairs

    // Calculate sums
    double sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0;
    double sum_x_squared = 0.0, sum_y_squared = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
        sum_x += x_values[i];
        sum_y += y_values[i];
        sum_xy += x_values[i] * y_values[i];
        sum_x_squared += x_values[i] * x_values[i];
        sum_y_squared += y_values[i] * y_values[i];
    }

    // Calculate correlation coefficient
    double numerator = n * sum_xy - sum_x * sum_y;
    double denominator = sqrt((n * sum_x_squared - sum_x * sum_x) * (n * sum_y_squared - sum_y * sum_y));

    // Handle division by zero
    if (denominator == 0)
        return 0.0;

    double correlation = numerator / denominator;
    return correlation;
}

void writeOutputToCSV(const std::string &output_file, int n, double correlation)
{
    std::ofstream out_file(output_file);
    out_file << "Number of pairs,Correlation coefficient\n";
    out_file << n << "," << std::fixed << std::setprecision(4) << correlation << "\n";
    out_file.close();
}

int main()
{
    std::string input_file = "input.csv";   // Adjust the filename if necessary
    std::string output_file = "output.csv"; // Output file for results
    int n = 0;                              // Number of pairs

    // Calculate correlation
    double correlation = calculateCorrelation(input_file, n);

    // Write results to output.csv
    writeOutputToCSV(output_file, n, correlation);

    // Print results to console
    std::cout << "Number of pairs: " << n << "\n";
    std::cout << "Correlation coefficient: " << std::fixed << std::setprecision(4) << correlation << "\n";
    std::cout << "Result saved to " << output_file << std::endl;

    return 0;
}
