#include <bits/stdc++.h>
using namespace std;

float median(vector<int> &a)
{
    int size = a.size();
    if (size % 2 == 1)
        return a[size / 2];
    else
        return (a[(size / 2) - 1] + a[size / 2]) / 2.0;
}

float quartile1(vector<int> &v)
{
    int n = v.size();
    vector<int> first;
    if (v.size() % 2 == 0)
    {
        first.assign(v.begin(), v.begin() + n / 2);
    }
    else
    {
        first.assign(v.begin(), v.begin() + n / 2);
    }
    return median(first);
}

float quartile3(vector<int> &v)
{
    int n = v.size();
    vector<int> last;
    if (v.size() % 2 == 0)
        last.assign(v.begin() + n / 2, v.end());
    else
        last.assign(v.begin() + n / 2 + 1, v.end());

    return median(last);
}

void five_number_summary(vector<int> &arr, ofstream &out)
{
    sort(arr.begin(), arr.end());
    int n = arr.size();

    float min_val = arr[0];
    float q1 = quartile1(arr);
    float med = median(arr);
    float q3 = quartile3(arr);
    float max_val = arr[n - 1];

    float iqr = q3 - q1;
    float lower_bound = q1 - 1.5 * iqr;
    float upper_bound = q3 + 1.5 * iqr;

    out << "Minimum value: " << min_val << "\n";
    out << "Quartile1 value: " << q1 << "\n";
    out << "Median value: " << med << "\n";
    out << "Quartile3 value: " << q3 << "\n";
    out << "Maximum value: " << max_val << "\n";
    out << "Interquartile Range (IQR): " << iqr << "\n";
    out << "Lower Bound: " << lower_bound << "\n";
    out << "Upper Bound: " << upper_bound << "\n";

    vector<int> outliers;
    for (int val : arr)
    {
        if (val < lower_bound || val > upper_bound)
        {
            outliers.push_back(val);
        }
    }
    if (outliers.empty())
    {
        out << "Outliers: None\n";
    }
    else
    {
        out << "Outliers: ";
        for (int val : outliers)
        {
            out << val << " ";
        }
        out << "\n";
    }
}

int main()
{
    ifstream in("data.csv");

    ofstream out("output.csv");

    int i = 0;
    string line, mark;
    vector<int> arr;

    while (getline(in, line))
    {
        if (i == 0)
        { // first line is ignore
            i++;
            continue;
        }
        stringstream str(line);
        while (getline(str, mark, ','))
        {
            try
            {
                int x = stoi(mark);
                arr.push_back(x);
            }
            catch (const invalid_argument &)
            {
                out << "wrong data" << mark << endl;
                continue;
            }
        }
    }

    five_number_summary(arr, out);

        in.close();
    out.close();

    return 0;
}
