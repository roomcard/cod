#include <bits/stdc++.h>
using namespace std;

vector<set<string>> read_csv_transactions(const string &file_path)
{
    vector<set<string>> transactions;
    ifstream file(file_path);
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        set<string> transaction;
        string item;
        while (getline(iss, item, ','))
        {
            transaction.insert(item);
        }
        transactions.push_back(transaction);
    }
    return transactions;
}

set<set<string>> get_itemsets(const vector<set<string>> &transactions, int length)
{
    set<set<string>> itemsets;
    for (const auto &transaction : transactions)
    {
        if (transaction.size() >= length)
        {
            vector<string> items(transaction.begin(), transaction.end());
            vector<int> indices(items.size());
            iota(indices.begin(), indices.end(), 0);

            do
            {
                set<string> itemset;
                for (int i = 0; i < length; ++i)
                {
                    itemset.insert(items[indices[i]]);
                }
                itemsets.insert(itemset);
            } while (next_permutation(indices.begin(), indices.end()));
        }
    }
    return itemsets;
}

int get_support(const set<string> &itemset, const vector<set<string>> &transactions)
{
    int count = 0;
    for (const auto &transaction : transactions)
    {
        if (includes(transaction.begin(), transaction.end(), itemset.begin(), itemset.end()))
        {
            ++count;
        }
    }
    return count;
}

vector<map<set<string>, int>> get_frequent_itemsets(const vector<set<string>> &transactions, int min_support, int n)
{
    vector<map<set<string>, int>> itemsets;
    int level = 1;
    set<set<string>> candidate_itemsets = get_itemsets(transactions, level);

    while (!candidate_itemsets.empty())
    {
        map<set<string>, int> itemset_count;

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
                    ++itemset_count[itemset];
                }
            }
        }

        map<set<string>, int> frequent_itemsets;
        for (auto it = itemset_count.begin(); it != itemset_count.end(); ++it)
        {
            if (it->second >= min_support)
            {
                frequent_itemsets[it->first] = it->second;
            }
        }

        if (frequent_itemsets.empty())
        {
            break;
        }

        itemsets.push_back(frequent_itemsets);
        set<string> frequent_items;

        for (auto it = itemsets.back().begin(); it != itemsets.back().end(); ++it)
        {
            frequent_items.insert(it->first.begin(), it->first.end());
        }

        level++;
        candidate_itemsets.clear();
        for (auto it1 = frequent_items.begin(); it1 != frequent_items.end(); ++it1)
        {
            for (auto it2 = next(it1); it2 != frequent_items.end(); ++it2)
            {
                set<string> candidate = {*it1, *it2};
                candidate_itemsets.insert(candidate);
            }
        }

        if (candidate_itemsets.size() < level)
        {
            break; // No more candidate itemsets of the current level.
        }
    }
    return itemsets;
}

vector<tuple<set<string>, set<string>, double>> generate_association_rules(
    const vector<map<set<string>, int>> &frequent_itemsets, double min_confidence, const vector<set<string>> &transactions)
{
    vector<tuple<set<string>, set<string>, double>> rules;

    for (size_t level = 1; level < frequent_itemsets.size(); ++level)
    {
        for (auto it = frequent_itemsets[level].begin(); it != frequent_itemsets[level].end(); ++it)
        {
            const set<string> &itemset = it->first;
            int support = it->second;

            vector<string> items(itemset.begin(), itemset.end());
            for (size_t i = 0; i < items.size(); ++i)
            {
                set<string> subset = {items[i]};
                set<string> remainder = itemset;
                remainder.erase(items[i]);

                if (!remainder.empty())
                {
                    double subset_support = get_support(subset, transactions);
                    double confidence = static_cast<double>(support) / subset_support;
                    if (confidence >= min_confidence)
                    {
                        rules.emplace_back(subset, remainder, confidence);
                    }
                }
            }
        }
    }
    return rules;
}
int main()
{
    string file_path = "input.csv";
    auto transactions = read_csv_transactions(file_path);
    int n = transactions.size();

    double support_percentage, confidence_percentage;
    cout << "Enter the minimum support as a percentage (e.g., 50 for 50%): ";
    cin >> support_percentage;
    cout << "Enter the minimum confidence as a percentage (e.g., 70 for 70%): ";
    cin >> confidence_percentage;

    int min_support = ceil((support_percentage / 100) * n);
    double min_confidence = confidence_percentage / 100;

    auto frequent_itemsets = get_frequent_itemsets(transactions, min_support, n);

    auto rules = generate_association_rules(frequent_itemsets, min_confidence, transactions);

    ofstream output_file("output.csv");
    if (!output_file.is_open())
    {
        cerr << "Error opening output.csv for writing.\n";
        return 1;
    }

    if (!rules.empty())
    {
        output_file << "Antecedent,Consequent,Confidence\n"; // CSV header
        for (const auto &rule : rules)
        {
            const set<string> &antecedent = get<0>(rule);
            const set<string> &consequent = get<1>(rule);
            double confidence = get<2>(rule);
            output_file << "{";
            for (const auto &item : antecedent)
                output_file << item << " ";
            output_file << "}, {";
            for (const auto &item : consequent)
                output_file << item << " ";
            output_file << "}, " << fixed << setprecision(2) << confidence * 100 << "\n";
        }
        cout << "Association rules written to output.csv\n";
    }
    else
    {
        output_file << "No association rules found.\n";
        cout << "No association rules found.\n";
    }

    output_file.close();
    return 0;
}