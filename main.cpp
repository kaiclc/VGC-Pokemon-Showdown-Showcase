#include "PokemonSegTree.hpp"
#include "simdjson.h"
#include <iostream>
#include <vector>

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

unordered_map<string_view, int> pokemonRawCount(string jsonFilePath) {
    unordered_map<string_view, int> result;
    ondemand::parser parser;

    auto json = padded_string::load(jsonFilePath);
    ondemand::document doc = parser.iterate(json);
    ondemand::object root = doc.get_object();
    auto data = root["data"].get_object();

    for (simdjson::ondemand::field entry : data) {
        string_view pokemon_name = entry.unescaped_key(); // POKEMON NAME
        auto pokemonInfo = entry.value().get_object();
        int raw_count = pokemonInfo["Raw count"].get_uint64();
        result[pokemon_name] = raw_count;
    }

    return result;
};
void parseAllFiles(vector<int> &totalBattlesByMonth, vector<unordered_map<string_view, int>> freqMapByMonth) {
    vector<string> year = {"2014", "2015", "2016", "2017", "2018", "2019", "2020", "2021", "2022", "2023", "2024", "2025"};
    vector<string> month = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12"};
    cout << "parsing data..." << endl;
    totalBattlesByMonth.push_back(loadBattleTotal("2014/2014-11.json"));
    freqMapByMonth.push_back(pokemonRawCount("2014/2014-11.json"));
    totalBattlesByMonth.push_back(loadBattleTotal("2014/2014-12.json"));
    freqMapByMonth.push_back(pokemonRawCount("2014/2014-12.json"));
    for(int i = 1; i < year.size() - 1; i++) {
        for(int j = 0; j < month.size(); j++) {
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

int main() {

    //string fileFormat = year[0] + "/" + year[0] + "-" + month[10] + ".json";
    //cout << fileFormat << "\n";
    
    vector<int> totalBattlesByMonth;
    vector<unordered_map<string_view, int>> freqMapByMonth;
    //unordered_map<string_view, int> freqMap = pokemonRawCount("2014/2014-11.json");

    parseAllFiles(totalBattlesByMonth, freqMapByMonth);

    return 0;
}
