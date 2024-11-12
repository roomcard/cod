#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

// Function to calculate Gini Index
double calculate_gini(const std::vector<std::string> &data)
{
    std::unordered_map<std::string, int> label_counts;
    int total = data.size();

    for (const auto &label : data)
    {
        label_counts[label]++;
    }

    double gini = 1.0;
    for (const auto &pair : label_counts)
    {
        double prob = static_cast<double>(pair.second) / total;
        gini -= prob * prob;
    }

    return gini;
}

// Function to calculate Entropy
double calculate_entropy(const std::vector<std::string> &data)
{
    std::unordered_map<std::string, int> label_counts;
    int total = data.size();

    for (const auto &label : data)
    {
        label_counts[label]++;
    }

    double entropy = 0.0;
    for (const auto &pair : label_counts)
    {
        double prob = static_cast<double>(pair.second) / total;
        entropy -= prob * log2(prob);
    }

    return entropy;
}

// Function to calculate Gini Gain
double gini_gain(const std::vector<std::vector<std::string>> &dataset, int attribute_idx, int target_idx)
{
    std::vector<std::string> target_data;
    for (const auto &row : dataset)
    {
        target_data.push_back(row[target_idx]);
    }

    double overall_gini = calculate_gini(target_data);
    std::unordered_map<std::string, std::vector<std::string>> attribute_values;

    for (const auto &row : dataset)
    {
        attribute_values[row[attribute_idx]].push_back(row[target_idx]);
    }

    double attribute_gini = 0.0;
    for (const auto &pair : attribute_values)
    {
        double subset_size = pair.second.size();
        attribute_gini += (subset_size / dataset.size()) * calculate_gini(pair.second);
    }

    return overall_gini - attribute_gini;
}

// Function to calculate Information Gain
double information_gain(const std::vector<std::vector<std::string>> &dataset, int attribute_idx, int target_idx)
{
    std::vector<std::string> target_data;
    for (const auto &row : dataset)
    {
        target_data.push_back(row[target_idx]);
    }

    double overall_entropy = calculate_entropy(target_data);
    std::unordered_map<std::string, std::vector<std::string>> attribute_values;

    for (const auto &row : dataset)
    {
        attribute_values[row[attribute_idx]].push_back(row[target_idx]);
    }

    double attribute_entropy = 0.0;
    for (const auto &pair : attribute_values)
    {
        double subset_size = pair.second.size();
        attribute_entropy += (subset_size / dataset.size()) * calculate_entropy(pair.second);
    }

    return overall_entropy - attribute_entropy;
}

// Load data from CSV file
std::vector<std::vector<std::string>> load_data(const std::string &file_path, std::vector<std::string> &header)
{
    std::vector<std::vector<std::string>> data;
    std::ifstream file(file_path);
    std::string line;

    // Read header
    if (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, ','))
        {
            header.push_back(item);
        }
    }

    // Read data
    while (std::getline(file, line))
    {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, ','))
        {
            row.push_back(item);
        }
        data.push_back(row);
    }

    return data;
}

// Main function
int main()
{
    std::string input_file = "input.csv";   // Change to your CSV file path
    std::string output_file = "output.csv"; // Output file path
    std::vector<std::string> header;

    // Load data
    std::vector<std::vector<std::string>> data = load_data(input_file, header);

    // Get target attribute index
    std::string target_attribute;
    std::cout << "Available attributes: ";
    for (const auto &attr : header)
    {
        std::cout << attr << " ";
    }
    std::cout << "\nEnter the target column from above: ";
    std::cin >> target_attribute;

    int target_idx = std::find(header.begin(), header.end(), target_attribute) - header.begin();

    // Open output file
    std::ofstream output(output_file);
    output << "Attribute,Gini Gain,Information Gain\n"; // Write header to output file

    // Calculate Gini Gain and Information Gain for each attribute
    for (int idx = 0; idx < header.size(); idx++)
    {
        if (idx != target_idx)
        {
            double gini_g = gini_gain(data, idx, target_idx);
            double info_gain = information_gain(data, idx, target_idx);
            output << header[idx] << "," << std::fixed << std::setprecision(4) << gini_g << ","
                   << std::fixed << std::setprecision(4) << info_gain << "\n"; // Write results to output file
        }
    }

    output.close(); // Close the output file
    std::cout << "Results written to " << output_file << std::endl;

    return 0;
}
