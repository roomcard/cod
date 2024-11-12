#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <string>

struct DataRow
{
    std::unordered_map<std::string, std::string> attributes;
};

// Function to read CSV data
std::vector<DataRow> readCSV(const std::string &filename)
{
    std::vector<DataRow> data;
    std::ifstream file(filename);
    std::string line;

    // Read the header to get attribute names
    std::getline(file, line);
    std::stringstream header_stream(line);
    std::vector<std::string> attribute_names;
    std::string attribute;
    while (std::getline(header_stream, attribute, ','))
    {
        attribute_names.push_back(attribute);
    }

    // Read each line of the CSV file
    while (std::getline(file, line))
    {
        std::stringstream line_stream(line);
        DataRow row;
        for (const auto &attr_name : attribute_names)
        {
            std::string value;
            std::getline(line_stream, value, ',');
            row.attributes[attr_name] = value;
        }
        data.push_back(row);
    }

    return data;
}

// Function to calculate entropy of the target attribute
double calculateEntropy(const std::vector<DataRow> &data, const std::string &target_attribute)
{
    std::unordered_map<std::string, int> value_count;
    int total_count = data.size();

    // Count occurrences of each target value
    for (const auto &row : data)
    {
        const auto &value = row.attributes.at(target_attribute);
        value_count[value]++;
    }

    // Calculate entropy
    double ent = 0.0;
    for (auto &pair : value_count)
    {
        const auto &count = pair.second;
        double probability = static_cast<double>(count) / total_count;
        if (probability > 0)
        {
            ent -= probability * log2(probability);
        }
    }

    return ent;
}

// Function to calculate information gain
double calculateInformationGain(const std::vector<DataRow> &data, const std::string &attribute, const std::string &target_attribute)
{
    double total_entropy = calculateEntropy(data, target_attribute);
    std::unordered_map<std::string, std::vector<DataRow>> partitions;

    // Partition data by attribute values
    for (const auto &row : data)
    {
        const auto &attr_value = row.attributes.at(attribute);
        partitions[attr_value].push_back(row);
    }

    // Calculate weighted entropy of the attribute
    double weighted_entropy = 0.0;
    int total_count = data.size();

    for (auto &pair : partitions)
    {
        const auto &subset = pair.second;
        double subset_entropy = calculateEntropy(subset, target_attribute);
        weighted_entropy += (static_cast<double>(subset.size()) / total_count) * subset_entropy;
    }

    // Calculate information gain
    return total_entropy - weighted_entropy;
}

// Function to write results to CSV
void writeResultToCSV(const std::string &output_file, const std::vector<std::pair<std::string, double>> &results)
{
    std::ofstream out_file(output_file);
    out_file << "Attribute,Information Gain\n";
    for (const auto &result : results)
    {
        out_file << result.first << "," << result.second << "\n";
    }
    out_file.close();
}

int main()
{
    std::string input_file = "input.csv";
    std::string output_file = "output.csv";
    std::vector<std::string> attribute_names = {"Weather", "Temperature", "Humidity"}; // List of attributes
    std::string target_column = "Play";                                                // Example target column

    // Read data from CSV file
    std::vector<DataRow> data = readCSV(input_file);

    std::vector<std::pair<std::string, double>> results; // Store results for each attribute

    // Calculate information gain for each attribute
    for (const auto &attribute_name : attribute_names)
    {
        double info_gain = calculateInformationGain(data, attribute_name, target_column);
        results.emplace_back(attribute_name, info_gain);
        std::cout << "Information Gain for the attribute '" << attribute_name << "' is: " << info_gain << std::endl;
    }

    // Write results to output CSV file
    writeResultToCSV(output_file, results);
    std::cout << "Results saved to " << output_file << std::endl;

    return 0;
}
