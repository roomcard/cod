#include <bits/stdc++.h>

using namespace std;

typedef vector<set<string>> Transactions;
typedef set<vector<string>> ItemSets;
typedef map<vector<string>, int> FrequentItemSets;

ItemSets get_itemsets(const set<string> &frequent_items, int length)
{
    ItemSets itemsets;
    vector<string> items(frequent_items.begin(), frequent_items.end());
    vector<bool> selector(items.size(), false);
    fill(selector.begin(), selector.begin() + length, true);

    do
    {
        vector<string> itemset;
        for (size_t i = 0; i < selector.size(); ++i)
        {
            if (selector[i])
            {
                itemset.push_back(items[i]);
            }
        }
        itemsets.insert(itemset);
    } while (prev_permutation(selector.begin(), selector.end()));

    return itemsets;
}

vector<FrequentItemSets> get_frequent_itemsets(const Transactions &transactions, double min_support, int n)
{
    vector<FrequentItemSets> itemsets;
    int level = 1;
    ofstream outFile("output.csv");
    outFile << "Level,Itemset,Count\n";

    set<string> all_items;
    for (const auto &transaction : transactions)
    {
        all_items.insert(transaction.begin(), transaction.end());
    }

    ItemSets candidate_itemsets = get_itemsets(all_items, level);

    while (!candidate_itemsets.empty())
    {
        map<vector<string>, int> itemset_count;

        for (const auto &itemset : candidate_itemsets)
        {
            itemset_count[itemset] = 0;
        }

        for (const auto &transaction : transactions)
        {
            for (const auto &itemset : candidate_itemsets)
            {
                if (includes(transaction.begin(), transaction.end(), itemset.begin(), itemset.end()))
                {
                    itemset_count[itemset]++;
                }
            }
        }

        FrequentItemSets frequent_itemsets;
        for (const auto &pair : itemset_count)
        {
            if (pair.second >= (min_support * n))
            {
                frequent_itemsets[pair.first] = pair.second;
            }
        }

        if (frequent_itemsets.empty())
        {
            break;
        }

        for (const auto &pair : frequent_itemsets)
        {
            outFile << level << ",";
            for (const auto &item : pair.first)
            {
                outFile << item << " ";
            }
            outFile << "," << pair.second << endl;
        }

        itemsets.push_back(frequent_itemsets);

        set<string> frequent_items;
        for (const auto &pair : frequent_itemsets)
        {
            frequent_items.insert(pair.first.begin(), pair.first.end());
        }

        level++;
        if (level > frequent_items.size())
        {
            break;
        }
        candidate_itemsets = get_itemsets(frequent_items, level);
    }

    outFile.close();
    return itemsets;
}

Transactions read_csv_transactions(const string &file_path)
{
    Transactions transactions;
    ifstream file(file_path);
    string line;

    while (getline(file, line))
    {
        istringstream iss(line);
        string item;
        set<string> transaction;

        while (getline(iss, item, ','))
        {
            if (!item.empty())
            {
                transaction.insert(item);
            }
        }

        if (!transaction.empty())
        {
            transactions.push_back(transaction);
        }
    }

    return transactions;
}

int main()
{
    string file_path = "data.csv";
    Transactions transactions = read_csv_transactions(file_path);
    int n = transactions.size();
    double min_support;

    cout << "Enter the minimum support: ";
    cin >> min_support;

    get_frequent_itemsets(transactions, min_support, n);

    cout << "Frequent itemsets have been written to output.csv" << endl;

    return 0;
}