#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

// Function to load dataset from a CSV file
void load_csv(const string &file_path, vector<vector<string>> &dataset, vector<string> &headers)
{
    ifstream file(file_path);
    string line;

    // Read the header
    if (getline(file, line))
    {
        stringstream ss(line);
        string header;
        while (getline(ss, header, ','))
        {
            headers.push_back(header);
        }
    }

    // Read the dataset
    while (getline(file, line))
    {
        stringstream ss(line);
        string value;
        vector<string> row;
        while (getline(ss, value, ','))
        {
            row.push_back(value);
        }
        dataset.push_back(row);
    }
}

// Function to separate the dataset by class
map<string, vector<vector<string>>> separate_by_class(const vector<vector<string>> &dataset, int class_index)
{
    map<string, vector<vector<string>>> separated;
    for (const auto &row : dataset)
    {
        string class_value = row[class_index];
        separated[class_value].push_back(row);
    }
    return separated;
}

// Function to calculate mean
double mean(const vector<double> &numbers)
{
    double sum = accumulate(numbers.begin(), numbers.end(), 0.0);
    return sum / numbers.size();
}

// Function to calculate variance
double variance(const vector<double> &numbers)
{
    double avg = mean(numbers);
    double var = 0.0;
    for (const auto &num : numbers)
    {
        var += (num - avg) * (num - avg);
    }
    return var / numbers.size();
}

// Function to calculate Gaussian probability
double gaussian_probability(double x, double mean, double var)
{
    var = max(var, 1e-9); // Prevent division by zero
    double exponent = exp(-((x - mean) * (x - mean)) / (2 * var));
    return (1.0 / (sqrt(2 * M_PI * var))) * exponent;
}

// Function to summarize dataset statistics (mean, variance) by class
map<string, vector<pair<double, double>>> summarize_by_class(const vector<vector<string>> &dataset, int class_index)
{
    auto separated = separate_by_class(dataset, class_index);
    map<string, vector<pair<double, double>>> summaries;

    for (const auto &pair : separated)
    {
        const string &class_value = pair.first;
        const auto &rows = pair.second;

        for (size_t i = 0; i < rows[0].size() - 1; ++i)
        { // Skip class column
            vector<double> column_data;
            for (const auto &row : rows)
            {
                column_data.push_back(stod(row[i]));
            }
            double mean_value = mean(column_data);
            double variance_value = variance(column_data);
            summaries[class_value].emplace_back(mean_value, variance_value);
        }
    }
    return summaries;
}

// Function to calculate class probabilities for a given input
map<string, double> calculate_class_probabilities(const map<string, vector<pair<double, double>>> &summaries, const vector<double> &input_row)
{
    map<string, double> probabilities;

    for (const auto &pair : summaries)
    {
        const string &class_value = pair.first;
        const auto &class_summaries = pair.second;
        probabilities[class_value] = 1.0;

        for (size_t i = 0; i < class_summaries.size(); ++i)
        {
            double mean = class_summaries[i].first;
            double var = class_summaries[i].second;
            probabilities[class_value] *= gaussian_probability(input_row[i], mean, var);
        }
    }
    return probabilities;
}

// Function to predict the class for a given input row
string predict(const map<string, vector<pair<double, double>>> &summaries, const vector<double> &input_row)
{
    auto probabilities = calculate_class_probabilities(summaries, input_row);
    string best_label;
    double best_prob = -1;

    for (const auto &pair : probabilities)
    {
        if (best_label.empty() || pair.second > best_prob)
        {
            best_prob = pair.second;
            best_label = pair.first;
        }
    }
    return best_label;
}

// Function to write output to CSV file
void write_output(const string &file_path, const vector<double> &input_row, const string &predicted_class)
{
    ofstream file(file_path);
    file << "Input,Predicted Class\n";
    file << "{";
    for (const auto &val : input_row)
    {
        file << val << ",";
    }
    file.seekp(-1, ios_base::cur); // Remove the last comma
    file << "}," << predicted_class << "\n";
}

int main()
{
    string file_path = "input.csv";
    vector<vector<string>> dataset;
    vector<string> headers;

    // Load dataset from CSV file
    load_csv(file_path, dataset, headers);

    int class_index = headers.size() - 1; // The class label is in the last column

    // Summarize the training dataset
    auto summaries = summarize_by_class(dataset, class_index);

    // Input to classify (replace with actual values from your dataset)
    vector<double> input_row = {6.0, 3.0, 4.8, 1.8}; // Example input row

    // Predict the class for the input
    string predicted_class = predict(summaries, input_row);

    // Output the prediction to a CSV file
    write_output("output.csv", input_row, predicted_class);

    cout << "Predicted class for input { ";
    for (const auto &val : input_row)
    {
        cout << val << " ";
    }
    cout << "}: " << predicted_class << endl;

    return 0;
}
