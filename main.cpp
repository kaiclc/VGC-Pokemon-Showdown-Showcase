#include "PokemonSegTree.hpp"
#include "PokemonFenwickTree.hpp"
#include "simdjson.h"
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace simdjson;



// the fileFormat is supposed to be the parameter
// I should list them all out below in the main function
// it'll return the integer of a total battle count; should be the raw variable
int loadBattleTotal(string jsonFilePath) {
    ondemand::parser parser;
    auto json = padded_string::load(jsonFilePath);
    ondemand::document doc = parser.iterate(json);
    ondemand::object root = doc.get_object();
    auto info = root["info"];
    int result = info["number of battles"].get_uint64();
    return result;
};

unordered_map<string, int> pokemonRawCount(string jsonFilePath) {
    unordered_map<string, int> result;
    ondemand::parser parser;

    auto json = padded_string::load(jsonFilePath);
    ondemand::document doc = parser.iterate(json);
    ondemand::object root = doc.get_object();
    auto data = root["data"].get_object();

    for (simdjson::ondemand::field entry : data) {
        std::string pokemon_name(entry.unescaped_key().value()); // POKEMON NAME
        //cout << pokemon_name << endl;
        simdjson::ondemand::object pokemonInfo = entry.value().get_object();
        auto count_result = pokemonInfo["Raw count"].get_uint64();
        auto temp = count_result.value();
        //cout << temp << endl;
        result[pokemon_name] = static_cast<int>(temp);
    }
    return result;
};
void parseAllFiles(vector<int>& totalBattlesByMonth, vector<unordered_map<string, int>>& freqMapByMonth) {
    vector<string> year = { "2014", "2015", "2016", "2017", "2018", "2019", "2020", "2021", "2022", "2023", "2024", "2025" };
    vector<string> month = { "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12" };
    cout << "parsing data..." << endl;
    totalBattlesByMonth.push_back(loadBattleTotal("2014/2014-11.json"));
    freqMapByMonth.push_back(pokemonRawCount("2014/2014-11.json"));
    totalBattlesByMonth.push_back(loadBattleTotal("2014/2014-12.json"));
    freqMapByMonth.push_back(pokemonRawCount("2014/2014-12.json"));
    for (int i = 1; i < year.size() - 1; i++) {
        for (int j = 0; j < month.size(); j++) {
            string fileFormat = year[i] + "/" + year[i] + "-" + month[j] + ".json";
            totalBattlesByMonth.push_back(loadBattleTotal(fileFormat));
            freqMapByMonth.push_back(pokemonRawCount(fileFormat));
        }
    }
    totalBattlesByMonth.push_back(loadBattleTotal("2025/2025-01.json"));
    freqMapByMonth.push_back(pokemonRawCount("2025/2025-01.json"));
    totalBattlesByMonth.push_back(loadBattleTotal("2025/2025-02.json"));
    freqMapByMonth.push_back(pokemonRawCount("2025/2025-02.json"));
    totalBattlesByMonth.push_back(loadBattleTotal("2025/2025-03.json"));
    freqMapByMonth.push_back(pokemonRawCount("2025/2025-03.json"));
    cout << "done!" << endl;
};

unordered_map<string, vector<int>> unionFrqMap(vector<unordered_map<string, int>>& freqMapByMonth) {
    unordered_map<string, vector<int>> result;
    for (int month = 0; month < freqMapByMonth.size(); ++month) {
        auto& freqMap = freqMapByMonth[month];
        for (auto it = freqMap.begin(); it != freqMap.end(); ++it) {
            if (result.find(it->first) == result.end()) {
                result[it->first] = vector<int>(freqMapByMonth.size(), 0);
            }
            result[it->first][month] = it->second;
        }
    }
    return result;
};

int main() {

    //string fileFormat = year[0] + "/" + year[0] + "-" + month[10] + ".json";
    //cout << fileFormat << "\n";

    vector<int> totalBattlesByMonth;
    vector<unordered_map<string, int>> freqMapByMonth;
    //unordered_map<string, int> freqMap = pokemonRawCount("2014/2014-11.json");
    //cout << freqMap["Garchomp"] << endl;


    parseAllFiles(totalBattlesByMonth, freqMapByMonth);
    const unordered_map<string, vector<int>> realCounts = unionFrqMap(freqMapByMonth);
    
    PokemonSegTree segTree(totalBattlesByMonth, realCounts);
    PokemonFenwickTree fenwickTree(totalBattlesByMonth, realCounts);

    cout << "Welcome to the VGC Pokemon Showdown Showcase!" << endl;
    cout << "The Purpose of the program is to demonstrate the Pokemon Usage in VGC competitive throughout Pokemon Showdown's history" << endl;
    cout << "Select a Pokemon and the time range" << endl;
    while (true) {
        string pokemonName;
        int startYear;
        int startMonth;
        int endYear;
        int endMonth;
        int startIndex;
        int endIndex;
        cout << "Enter a Pokemon name ('quit' to exit): ";
        cin >> pokemonName;
        if (pokemonName == "quit") {
            break;
        }
        cout << "Enter start year: ";
        cin >> startYear;
        if (startYear < 2014 || startYear>2025) {
            cout << "Invalid Year. Try Again" << endl << endl;
            continue;
        }
        cout << "Enter start month: ";
        cin >> startMonth;
        if (startMonth < 1 || startMonth>12) {
            cout << "Invalid Month. Try Again" << endl << endl;
            continue;
        }
        startIndex = (startYear - 2014) * 12 + startMonth - 11;
        cout << "Enter end year: ";
        cin >> endYear;
        if (endYear < 2014 || endYear>2025) {
            cout << "Invalid Year. Try Again" << endl << endl;
            continue;
        }
        cout << "Enter end month: ";
        cin >> endMonth;
        if (endMonth < 1 || endMonth>12) {
            cout << "Invalid Month. Try Again" << endl << endl;
            continue;
        }
        endIndex = (endYear - 2014) * 12 + endMonth - 11;

        auto startSeg = chrono::high_resolution_clock::now();
        auto segGroup = segTree.getUsage(pokemonName, startIndex, endIndex);
        auto endSeg = chrono::high_resolution_clock::now();

        auto startFenwick = chrono::high_resolution_clock::now();
        auto fenwickGroup = fenwickTree.getUsage(pokemonName, startIndex, endIndex);
        auto endFenwick = chrono::high_resolution_clock::now();

        cout << "\nResults for " << pokemonName << ":" << endl;
        cout << "   Usage count: " << get<0>(segGroup) << endl;
        cout << "   Total Battles: " << get<1>(segGroup) << endl;
        cout << "   Usage Percentage: " << fixed << setprecision(2) << (100.0 * get<0>(segGroup) / (get<1>(segGroup))) << endl;
        // test to ensure segment and fenwick arrive at same result
        if (get<0>(segGroup) == get<0>(fenwickGroup) && get<1>(segGroup) == get<1>(fenwickGroup)) {
            cout << "      Segment and Fenwick results match!" << endl;
        } else {
            cout << "      ERROR: Segment and Fenwick results do not match!" << endl;
        }

        cout << "Performance: " << endl;
        cout << "   Segment Tree: " << chrono::duration_cast<chrono::microseconds>(endSeg - startSeg).count() << "microseconds" << endl;
        cout << "   Fenwick Tree: " << chrono::duration_cast<chrono::microseconds>(endFenwick - startFenwick).count() << "microseconds" << endl << endl;
    }
    return 0;
}
