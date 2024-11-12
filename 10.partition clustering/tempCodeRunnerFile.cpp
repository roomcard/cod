#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <string>

using namespace std;

// Calculate distance between two points
double dist(const vector<double> &p1, const vector<double> &p2)
{
    double d = 0;
    for (size_t i = 0; i < p1.size(); i++)
    {
        d += pow(p1[i] - p2[i], 2);
    }
    return sqrt(d);
}

// Calculate mean of a cluster
vector<double> mean(const vector<vector<double>> &cluster)
{
    size_t n = cluster[0].size();
    vector<double> m(n, 0);

    for (const auto &p : cluster)
    {
        for (size_t i = 0; i < n; i++)
        {
            m[i] += p[i];
        }
    }

    for (double &x : m)
    {
        x /= cluster.size();
    }
    return m;
}

// K-means clustering algorithm
pair<vector<vector<vector<double>>>, vector<vector<double>>> kmeans(
    const vector<vector<double>> &data,
    vector<vector<double>> centroids,
    int max_iters = 100)
{

    int k = centroids.size();
    vector<vector<vector<double>>> clusters(k);

    for (int iteration = 0; iteration < max_iters; iteration++)
    {
        // Clear clusters for this iteration
        for (auto &cluster : clusters)
        {
            cluster.clear();
        }

        // Assign points to nearest centroid
        for (const auto &p : data)
        {
            vector<double> dists;
            for (const auto &c : centroids)
            {
                dists.push_back(dist(p, c));
            }
            int idx = min_element(dists.begin(), dists.end()) - dists.begin();
            clusters[idx].push_back(p);
        }

        // Calculate new centroids
        vector<vector<double>> new_centroids;
        for (size_t i = 0; i < k; i++)
        {
            if (!clusters[i].empty())
            {
                new_centroids.push_back(mean(clusters[i]));
            }
            else
            {
                new_centroids.push_back(centroids[i]); // Retain old centroid if no points are assigned
            }
        }

        // Print current iteration status
        cout << "\nIteration " << iteration + 1 << ":" << endl;
        for (size_t i = 0; i < clusters.size(); i++)
        {
            cout << "Cluster " << i + 1 << ": ";
            for (const auto &point : clusters[i])
            {
                cout << "(";
                for (size_t j = 0; j < point.size(); j++)
                {
                    cout << point[j];
                    if (j < point.size() - 1)
                        cout << ",";
                }
                cout << ") ";
            }
            cout << endl;
        }

        // Print centroids
        cout << "Centroids: ";
        for (const auto &c : new_centroids)
        {
            cout << "(";
            for (size_t j = 0; j < c.size(); j++)
            {
                cout << c[j];
                if (j < c.size() - 1)
                    cout << ",";
            }
            cout << ") ";
        }
        cout << endl;

        // Check for convergence
        if (new_centroids == centroids)
        {
            break;
        }
        centroids = new_centroids;
    }
    return make_pair(clusters, centroids);
}

// Read data from CSV file
vector<vector<double>> read_csv(const string &filename)
{
    vector<vector<double>> data;
    ifstream file(filename);
    string line;

    // Skip header
    getline(file, line);

    // Read data points
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string value;
        vector<double> point;

        while (getline(ss, value, ','))
        {
            point.push_back(stod(value));
        }
        data.push_back(point);
    }
    return data;
}

// Write clusters and centroids to CSV file
void write_output_csv(const string &filename, const vector<vector<vector<double>>> &clusters, const vector<vector<double>> &centroids)
{
    ofstream file(filename);

    if (!file.is_open())
    {
        cout << "Failed to open the file: " << filename << endl;
        return;
    }

    file << "Final Clusters\n"; // CSV Header for clusters
    for (size_t i = 0; i < clusters.size(); i++)
    {
        file << "Cluster " << i + 1 << ": ";
        for (const auto &point : clusters[i])
        {
            file << "(";
            for (size_t j = 0; j < point.size(); j++)
            {
                file << point[j];
                if (j < point.size() - 1)
                    file << ",";
            }
            file << ") ";
        }
        file << endl;
    }

    file << "\nFinal Centroids\n"; // CSV Header for centroids
    for (const auto &c : centroids)
    {
        file << "(";
        for (size_t i = 0; i < c.size(); i++)
        {
            file << c[i];
            if (i < c.size() - 1)
                file << ",";
        }
        file << ")\n"; // New line for each centroid
    }

    file.close(); // Close file after writing
    cout << "Results written to " << filename << endl;
}

int main()
{
    string filename = "data.csv"; // Update with your file path
    vector<vector<double>> data = read_csv(filename);

    bool can_we_stop = true;
    while (can_we_stop)
    {
        int k;
        cout << "Enter the number of clusters (k): ";
        cin >> k;

        vector<vector<double>> centroids;
        for (int i = 0; i < k; i++)
        {
            cout << "Centroid " << i + 1 << ": ";
            vector<double> centroid;
            double value;

            // Read space-separated values for centroid
            string line;
            cin.ignore(); // Clear the buffer
            getline(cin, line);
            stringstream ss(line);
            while (ss >> value)
            {
                centroid.push_back(value);
            }
            centroids.push_back(centroid);
        }

        // Perform k-means clustering
        pair<vector<vector<vector<double>>>, vector<vector<double>>> result = kmeans(data, centroids);
        vector<vector<vector<double>>> clusters = result.first;
        vector<vector<double>> final_centroids = result.second;

        // Write final results to output.csv
        write_output_csv("output.csv", clusters, final_centroids);

        char stop;
        cout << "Can we stop? Enter 'y' for yes and 'n' for no: ";
        cin >> stop;

        if (stop == 'y')
        {
            can_we_stop = false;
        }
    }

    return 0;
}
