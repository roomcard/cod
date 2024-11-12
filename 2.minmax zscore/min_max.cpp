#include <bits/stdc++.h>
#include <fstream>
using namespace std;

void min_max_normalization(ifstream &minmax, ofstream &minmaxop)
{
    double value, min_val, max_val, new_min, new_max;

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

    cout << "Enter the new minimum value=> ";
    cin >> new_min;
    cout << "Enter the new maximum value=>  ";
    cin >> new_max;

    minmax.clear();
    minmax.seekg(0);
    minmax >> value;

    while (minmax)
    {
        double normalized_val = (((value - min_val) / (max_val - min_val)) * (new_max - new_min)) + new_min;
        minmaxop << value << "," << normalized_val << "\n";
        minmax >> value;
    }

    cout << "success.\n";
}

int main()
{
    ifstream minmax("minmax.csv");

    ofstream minmaxop("minmaxop.csv");
    min_max_normalization(minmax, minmaxop);

    return 0;
}
