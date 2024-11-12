#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <string>
#include <iomanip>
#include <algorithm> // For min_element

using namespace std;

// Calculate the Euclidean distance between two points
double dist(const vector<double> &p1, const vector<double> &p2)
{
    double d = 0;
    for (size_t i = 0; i < p1.size(); i++)
    {
        d += pow(p1[i] - p2[i], 2);
    }
    return sqrt(d);
}

// Calculate the mean of points in a cluster
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
    const vector<vector<double>> &data, vector<vector<double>> centroids, int max_iters = 100)
{

    int k = centroids.size();
    vector<vector<vector<double>>> clusters;

    for (int iteration = 0; iteration < max_iters; iteration++)
    {
        clusters = vector<vector<vector<double>>>(k); // Reinitialize clusters for each iteration

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
                new_centroids.push_back(centroids[i]); // Retain old centroid if cluster is empty
            }
        }

        cout << "\nIteration " << iteration + 1 << ":\n";
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

    // Skip the header
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

int main()
{
    string filename = "exp10_kmeans.csv";
    vector<vector<double>> data = read_csv(filename);

    int k;
    cout << "Enter the number of clusters (k): ";
    cin >> k;

    vector<vector<double>> centroids;
    for (int i = 0; i < k; i++)
    {
        cout << "Enter Centroid " << i + 1 << ": ";
        vector<double> centroid;
        double value;

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
    auto result = kmeans(data, centroids);
    vector<vector<vector<double>>> clusters = result.first;
    vector<vector<double>> final_centroids = result.second;

    cout << "\nFinal Clusters:\n";
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

    cout << "\nFinal Centroids:\n";
    for (const auto &c : final_centroids)
    {
        cout << "(";
        for (size_t j = 0; j < c.size(); j++)
        {
            cout << c[j];
            if (j < c.size() - 1)
                cout << ",";
        }
        cout << ")\n";
    }

    return 0;
}
