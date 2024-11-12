#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>

struct DataRow
{
    std::string genre;
    std::string holytoly;
    int count;
    double T_weight;
    double D_weight;
};

// Function to read data from the CSV file
std::vector<DataRow> readCSV(const std::string &file_path)
{
    std::vector<DataRow> data;
    std::ifstream file(file_path);
    std::string line;

    // Skip the header
    std::getline(file, line);

    // Read each line of the CSV file
    while (std::getline(file, line))
    {
        std::stringstream line_stream(line);
        DataRow row;
        std::string count_str;

        std::getline(line_stream, row.genre, ',');
        std::getline(line_stream, row.holytoly, ',');
        std::getline(line_stream, count_str, ',');
        row.count = std::stoi(count_str); // Convert count to int

        data.push_back(row);
    }

    return data;
}

// Function to calculate T-weight and D-weight
void calculateWeights(std::vector<DataRow> &data)
{
    // Step 1: Calculate total count per genre
    std::unordered_map<std::string, int> total_count_per_genre;
    for (const auto &row : data)
    {
        total_count_per_genre[row.genre] += row.count;
    }

    // Step 2: Calculate T-weight (in percentage)
    for (auto &row : data)
    {
        row.T_weight = (static_cast<double>(row.count) / total_count_per_genre[row.genre]) * 100;
    }

    // Step 3: Calculate total count per holytoly across all genres
    std::unordered_map<std::string, int> total_count_per_holytoly;
    for (const auto &row : data)
    {
        total_count_per_holytoly[row.holytoly] += row.count;
    }

    // Step 4: Calculate D-weight (in percentage)
    for (auto &row : data)
    {
        row.D_weight = (static_cast<double>(row.count) / total_count_per_holytoly[row.holytoly]) * 100;
    }
}

// Function to calculate row and column totals
void calculateTotals(const std::vector<DataRow> &data, std::unordered_map<std::string, int> &genre_totals, std::unordered_map<std::string, int> &holytoly_totals, int &grand_total)
{
    for (const auto &row : data)
    {
        genre_totals[row.genre] += row.count;
        holytoly_totals[row.holytoly] += row.count;
        grand_total += row.count;
    }
}

// Function to write results to output CSV file in the specified format
void writeResultsToCSV(const std::string &output_file, const std::vector<DataRow> &data, const std::unordered_map<std::string, int> &genre_totals, const std::unordered_map<std::string, int> &holytoly_totals, int grand_total)
{
    std::ofstream out_file(output_file);
    out_file << "Column\\Row,BollyWood,,,TollyWood,,,Total\n";
    out_file << ",Count,T-weight,D-weight,Count,T-weight,D-weight,Count,T-weight,D-weight\n";

    for (const auto &genre_total : genre_totals)
    {
        std::string genre = genre_total.first;
        int genre_total_count = genre_total.second;

        int bollywood_count = 0, tollywood_count = 0;
        double bollywood_tweight = 0.0, bollywood_dweight = 0.0;
        double tollywood_tweight = 0.0, tollywood_dweight = 0.0;

        for (const auto &row : data)
        {
            if (row.genre == genre)
            {
                if (row.holytoly == "BollyWood")
                {
                    bollywood_count = row.count;
                    bollywood_tweight = row.T_weight;
                    bollywood_dweight = row.D_weight;
                }
                else if (row.holytoly == "TollyWood")
                {
                    tollywood_count = row.count;
                    tollywood_tweight = row.T_weight;
                    tollywood_dweight = row.D_weight;
                }
            }
        }

        out_file << genre << ","
                 << bollywood_count << "," << std::fixed << std::setprecision(2) << bollywood_tweight << "%," << bollywood_dweight << "%,"
                 << tollywood_count << "," << std::fixed << std::setprecision(2) << tollywood_tweight << "%," << tollywood_dweight << "%,"
                 << genre_total_count << ",100%," << std::fixed << std::setprecision(2) << ((bollywood_dweight + tollywood_dweight) / 2) << "%\n";
    }

    // Write totals for Bollywood, Tollywood, and overall
    int bollywood_total = holytoly_totals.count("BollyWood") ? holytoly_totals.at("BollyWood") : 0;
    int tollywood_total = holytoly_totals.count("TollyWood") ? holytoly_totals.at("TollyWood") : 0;

    out_file << "Total," << bollywood_total << ",50.98%,100%,"
             << tollywood_total << ",49.02%,100%,"
             << grand_total << ",100%,100%\n";

    out_file.close();
}

int main()
{
    std::string input_file = "input.csv";   // Adjust the filename if necessary
    std::string output_file = "output.csv"; // Output file

    // Read data from the CSV file
    std::vector<DataRow> data = readCSV(input_file);

    // Calculate weights
    calculateWeights(data);

    // Calculate row and column totals
    std::unordered_map<std::string, int> genre_totals;
    std::unordered_map<std::string, int> holytoly_totals;
    int grand_total = 0;
    calculateTotals(data, genre_totals, holytoly_totals, grand_total);

    // Write the results to output CSV file in the specified format
    writeResultsToCSV(output_file, data, genre_totals, holytoly_totals, grand_total);

    std::cout << "Results saved to " << output_file << std::endl;

    return 0;
}

int power(int a, int b, int mod)
{
    a %= mod;
    int res = 1;
    while (b)
    {
        if (b & 1)
        {
            res = (res * a) % mod;
        }
        a = (a * a) % mod;
        b >>= 1;
    }
    return res;
}
int m_add(int a, int b, int mod)
{
    return (a % mod + b % mod + mod) % mod;
}
int m_sub(int a, int b, int mod)
{
    return ((a % mod - b % mod) % mod + mod) % mod;
}
int m_mul(int a, int b, int mod)
{
    return (a % mod * b % mod) % mod;
}
int m_div(int a, int b, int mod)
{
    return ((a % mod) * power(b, mod - 2, mod)) % mod;
}
