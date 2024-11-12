#include <bits/stdc++.h>
#include <fstream>
using namespace std;

void min_max_normalization(ifstream &minmax, ofstream &minmaxop)
{
    double value, min_val, max_val, new_min, new_max;

    // Finding the min and max values in the dataset
    minmax >> value;
    min_val = max_val = value;
    while (minmax)
    {
        if (value > max_val)
            max_val = value;
        if (value < min_val)
            min_val = value;
        minmax >> value;
    }

    // Input new min and max for normalization
    cout << "Enter the new minimum value: ";
    cin >> new_min;
    cout << "Enter the new maximum value: ";
    cin >> new_max;

    // Re-open file for normalization process
    minmax.clear();
    minmax.seekg(0);
    minmax >> value;

    // Perform Min-Max normalization and write to output
    while (minmax)
    {
        double normalized_val = (((value - min_val) / (max_val - min_val)) * (new_max - new_min)) + new_min;
        minmaxop << value << "," << normalized_val << "\n";
        minmax >> value;
    }

    cout << "Min-Max Normalization completed successfully! The output has been written to 'minmaxop.csv'.\n";
}

int main()
{
    ifstream minmax("minmax.csv");
    if (!minmax)
    {
        cout << "Error: Unable to open the input file (minmax.csv).\n";
        return 1;
    }

    ofstream minmaxop("minmaxop.csv", ios::trunc);
    min_max_normalization(minmax, minmaxop);

    return 0;
}
