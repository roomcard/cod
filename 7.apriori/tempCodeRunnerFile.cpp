#include <bits/stdc++.h>
#include <map>
using namespace std;

ifstream fin;
vector<set<string>> datatable;
set<string> products;
map<string, int> freq;

vector<string> wordsof(string str)
{
    vector<string> tmpset;
    string tmp = "";
    for (char c : str)
    {
        if (isalnum(c))
            tmp += c;
        else
        {
            if (!tmp.empty())
                tmpset.push_back(tmp);
            tmp.clear();
        }
    }
    if (!tmp.empty())
        tmpset.push_back(tmp);
    return tmpset;
}

string combine(const vector<string> &arr, int miss)
{
    string str;
    for (int i = 0; i < arr.size(); i++)
        if (i != miss)
            str += arr[i] + " ";
    str = str.substr(0, str.size() - 1);
    return str;
}

set<string> apriori_gen(set<string> &sets, int k)
{
    set<string> set2;
    for (auto it1 = sets.begin(); it1 != sets.end(); ++it1)
    {
        for (auto it2 = next(it1); it2 != sets.end(); ++it2)
        {
            vector<string> v1 = wordsof(*it1);
            vector<string> v2 = wordsof(*it2);
            bool alleq = true;

            for (int i = 0; i < k - 1 && alleq; i++)
                if (v1[i] != v2[i])
                    alleq = false;

            v1.push_back(v2[k - 1]);
            if (v1[v1.size() - 1] < v1[v1.size() - 2])
                swap(v1[v1.size() - 1], v1[v1.size() - 2]);
            for (int i = 0; i < v1.size() && alleq; i++)
            {
                string tmp = combine(v1, i);
                if (sets.find(tmp) == sets.end())
                    alleq = false;
            }

            if (alleq)
                set2.insert(combine(v1, -1));
        }
    }
    return set2;
}

int main()
{
    fin.open("data.csv");
    if (!fin.is_open())
    {
        cerr << "Error in opening input file" << endl;
        return 1;
    }

    string str;
    while (getline(fin, str))
    {
        vector<string> arr = wordsof(str);
        set<string> tmpset(arr.begin(), arr.end());
        datatable.push_back(tmpset);
        for (const auto &item : tmpset)
        {
            products.insert(item);
            freq[item]++;
        }
    }
    fin.close();

    cout << "No of transactions: " << datatable.size() << endl;

    int minfre = 1; // Default minimum frequency
    cout << "Min frequency: " << minfre << endl;

    queue<set<string>::iterator> q;
    for (auto it = products.begin(); it != products.end(); it++)
    {
        if (freq[*it] < minfre)
            q.push(it);
    }

    while (!q.empty())
    {
        products.erase(*q.front());
        q.pop();
    }

    ofstream fw("ferqitem_op.csv");
    if (!fw.is_open())
    {
        cerr << "Error in opening output file" << endl;
        return 1;
    }

    int pass = 1;
    fw << "Frequent " << pass++ << " item set:\n";
    for (const auto &item : products)
    {
        fw << "{" << item << "}, " << freq[item] << endl;
    }

    int i = 2;
    set<string> prev = products;
    while (true)
    {
        set<string> cur = apriori_gen(prev, i - 1);
        if (cur.empty())
            break;

        fw << "\nFrequent " << pass++ << " item set:\n";
        for (auto it = cur.begin(); it != cur.end();)
        {
            vector<string> arr = wordsof(*it);
            int tot = 0;
            for (const auto &transaction : datatable)
            {
                bool pres = all_of(arr.begin(), arr.end(), [&transaction](const string &word)
                                   { return transaction.find(word) != transaction.end(); });
                if (pres)
                    tot++;
            }

            if (tot >= minfre)
            {
                freq[*it] += tot;
                fw << "{" << *it << "}, " << freq[*it] << endl; // Write to file
                ++it;                                           // Increment iterator if item is frequent
            }
            else
            {
                it = cur.erase(it); // Remove item if not frequent
            }
        }

        if (cur.empty())
            break;
        prev = cur;
        i++;
    }

    fw.close(); // Ensure the output file is closed
    return 0;
}
