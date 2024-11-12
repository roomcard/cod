#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>

using namespace std;

// Function to load CSV data into a vector of vectors
vector<vector<double>> load_csv_data(const string &file_path)
{
    vector<vector<double>> data_entries;
    ifstream input_file(file_path);
    string line;

    // Skip header
    getline(input_file, line);

    // Process each line of data
    while (getline(input_file, line))
    {
        stringstream ss(line);
        string entry;
        vector<double> data_row;

        // Parse the line into individual elements
        while (getline(ss, entry, ','))
        {
            data_row.push_back(stod(entry));
        }
        data_entries.push_back(data_row);
    }
    return data_entries;
}

// Function to calculate Euclidean distance between two points
double calculate_euclidean(const vector<double> &point_a, const vector<double> &point_b)
{
    double sum_of_squares = 0;
    for (size_t i = 0; i < point_a.size(); ++i)
    {
        sum_of_squares += pow(point_a[i] - point_b[i], 2);
    }
    return sqrt(sum_of_squares);
}

// Single linkage to find minimum distance between clusters
double compute_single_linkage(const vector<vector<double>> &cluster_a, const vector<vector<double>> &cluster_b)
{
    double smallest_distance = numeric_limits<double>::max();
    for (const auto &p1 : cluster_a)
    {
        for (const auto &p2 : cluster_b)
        {
            double dist = calculate_euclidean(p1, p2);
            if (dist < smallest_distance)
            {
                smallest_distance = dist;
            }
        }
    }
    return smallest_distance;
}

// Hierarchical clustering with agglomerative strategy (single linkage)
vector<vector<double>> hierarchical_clustering(vector<vector<double>> &data_points)
{
    vector<vector<vector<double>>> active_clusters;

    // Initialize each point as its own cluster
    for (const auto &point : data_points)
    {
        active_clusters.push_back({point});
    }

    // Merge clusters iteratively until only one remains
    while (active_clusters.size() > 1)
    {
        double min_distance = numeric_limits<double>::max();
        int first_cluster = -1, second_cluster = -1;

        // Search for closest clusters
        for (size_t i = 0; i < active_clusters.size(); ++i)
        {
            for (size_t j = i + 1; j < active_clusters.size(); ++j)
            {
                double distance = compute_single_linkage(active_clusters[i], active_clusters[j]);
                if (distance < min_distance)
                {
                    min_distance = distance;
                    first_cluster = i;
                    second_cluster = j;
                }
            }
        }

        // Merge the identified clusters
        active_clusters[first_cluster].insert(active_clusters[first_cluster].end(),
                                              active_clusters[second_cluster].begin(),
                                              active_clusters[second_cluster].end());
        active_clusters.erase(active_clusters.begin() + second_cluster);
    }

    return active_clusters[0];
}

// Function to write the final cluster result to a CSV file
void write_output_csv(const vector<vector<double>> &cluster, const string &output_file)
{
    ofstream file(output_file);
    file << "X1,X2\n"; // CSV header
    for (const auto &point : cluster)
    {
        file << point[0] << "," << point[1] << "\n"; // Writing each point to the CSV
    }
    file.close();
}

int main()
{
    string file_path = "input.csv";
    string output_file = "output.csv";

    // Load data from the CSV file
    vector<vector<double>> data_entries = load_csv_data(file_path);

    // Perform hierarchical clustering
    vector<vector<double>> resulting_cluster = hierarchical_clustering(data_entries);

    // Write the final cluster result to a CSV file
    write_output_csv(resulting_cluster, output_file);

    return 0;
}