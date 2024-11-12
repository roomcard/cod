#include <bits/stdc++.h>
using namespace std;

void min_max(ifstream minmaxin, ofstream minmaxop)
{
    int new_min, new_max;
    cin >> new_min >> new_max;
    double val;

    double min_val = val, max_val = val;
    while (minmaxin)
    {
        minmaxin >> val;
        if (val > max_val)
        {
            max_val = val;
        }
        if (val < min_val)
        {
            min_val = val;
        }
    }
    minmaxin.clear();
    minmaxin.seekg(0);
    while (minmaxin)
    {
        minmaxin >> val;
        double normalized_val = (((val - min_val) / (max_val - min_val)) * (new_max - new_min)) + new_min;
        minmaxop << val << "," << normalized_val << "\n";
    }
}

int main()
{
    ifstream minmaxin("minmax.csv");
    ofstream minmaxout("minmaxop.csv");
}