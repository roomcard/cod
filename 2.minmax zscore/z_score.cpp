#include <bits/stdc++.h>
#include <fstream>
using namespace std;

void z_score_normalization(ifstream &zscore, ofstream &zscoreop)
{
    double value, total = 0, count = 0, sum_of_squares = 0, avg, std_dev;

    zscore >> value;
    while (zscore)
    {
        total += value;
        count++;
        zscore >> value;
    }
    avg = total / count;

    zscore.clear();
    zscore.seekg(0);

    zscore >> value;
    while (zscore)
    {
        sum_of_squares += (value - avg) * (value - avg);
        zscore >> value;
    }
    std_dev = sqrt(sum_of_squares / count);

    zscore.clear();
    zscore.seekg(0);

    zscore >> value;
    while (zscore)
    {
        double zscore_val = (value - avg) / std_dev;
        zscoreop << value << "," << zscore_val << "\n";
        zscore >> value;
    }

    zscoreop << "\nMean," << avg << "\n";
    zscoreop << "Standard Deviation," << std_dev << "\n";

    cout << "success.\n";
}

int main()
{
    ifstream zscore("zscore.csv");
    if (!zscore)
    {
        cout << "Error\n";
        return 1;
    }

    ofstream zscoreop("zscoreop.csv", ios::trunc);
    z_score_normalization(zscore, zscoreop);

    return 0;
}
