#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>

struct Data
{
    int ID;
    double Marks;
};

// Function to read CSV data
std::vector<Data> readCSV(const std::string &filename)
{
    std::vector<Data> data;
    std::ifstream file(filename);
    std::string line;

    // Ignore the first line (header)
    std::getline(file, line);

    // Read each line of the CSV file
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string id_str, marks_str;
        std::getline(ss, id_str, ',');
        std::getline(ss, marks_str, ',');

        Data entry;
        entry.ID = std::stoi(id_str);
        entry.Marks = std::stod(marks_str);
        data.push_back(entry);
    }

    return data;
}

// Function to calculate the mean of a bin
double calculateMean(const std::vector<double> &bin_data)
{
    double sum = 0;
    for (double value : bin_data)
        sum += value;
    return sum / bin_data.size();
}

// Function to calculate the median of a bin
double calculateMedian(std::vector<double> bin_data)
{
    std::sort(bin_data.begin(), bin_data.end());
    size_t size = bin_data.size();
    if (size % 2 == 0)
        return (bin_data[size / 2 - 1] + bin_data[size / 2]) / 2.0;
    return bin_data[size / 2];
}

// Function to partition data into bins
std::vector<std::vector<double>> createBins(std::vector<Data> &data, int num_bins)
{
    std::vector<std::vector<double>> bins;
    int bin_size = data.size() / num_bins;
    int remainder = data.size() % num_bins;
    int start_index = 0;

    // Sort data by Marks
    struct compare
    {
        bool operator()(Data &a, const Data &b)
        {
            return a.Marks < b.Marks;
        }
    };

    std::sort(data.begin(), data.end(), compare());

    for (int i = 0; i < num_bins; ++i)
    {
        int current_bin_size = bin_size + (i < remainder ? 1 : 0);
        std::vector<double> bin_data;

        for (int j = start_index; j < start_index + current_bin_size; ++j)
        {
            bin_data.push_back(data[j].Marks);
        }

        bins.push_back(bin_data);
        start_index += current_bin_size;
    }

    return bins;
}

// Function to write bins and their transformations to a CSV file
void writeBinsToCSV(const std::vector<std::vector<double>> &bins, const std::string &filename)
{
    std::ofstream file(filename);
    file << "Bin,Method,Value\n";

    // Write bins by Mean
    for (size_t i = 0; i < bins.size(); ++i)
    {
        double mean = calculateMean(bins[i]);
        for (size_t j = 0; j < bins[i].size(); ++j)
        {
            file << "Bin " << i + 1 << ",Mean," << mean << "\n";
        }
    }

    // Write bins by Median
    for (size_t i = 0; i < bins.size(); ++i)
    {
        double median = calculateMedian(bins[i]);
        for (size_t j = 0; j < bins[i].size(); ++j)
        {
            file << "Bin " << i + 1 << ",Median," << median << "\n";
        }
    }

    // Write bins by Boundaries
    for (size_t i = 0; i < bins.size(); ++i)
    {
        double lower_bound = *std::min_element(bins[i].begin(), bins[i].end());
        double upper_bound = *std::max_element(bins[i].begin(), bins[i].end());
        for (double value : bins[i])
        {
            file << "Bin " << i + 1 << ",Boundaries," << (value <= lower_bound ? lower_bound : upper_bound) << "\n";
        }
    }

    file.close();
}

int main()
{
    std::string input_file = "input.csv";
    std::string output_file = "output.csv";
    int num_bins;

    std::cout << "Enter the number of bins to create: ";
    std::cin >> num_bins;

    std::vector<Data> data = readCSV(input_file);

    // Create bins
    std::vector<std::vector<double>> bins = createBins(data, num_bins);

    // Write bins and transformations to CSV file
    writeBinsToCSV(bins, output_file);

    std::cout << "Output written to " << output_file << std::endl;

    return 0;
}
