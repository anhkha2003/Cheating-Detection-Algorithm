#include<bits/stdc++.h>
#include "finding_pattern.h"
using namespace std;
using namespace CountPattern;
namespace fs = std::filesystem;

vector<string> specialString = {"'YES'", "'Yes'", "'yes'", 
                                "'IMPOSSIBLE'", "\"Yes\"", "\"Yes\"", 
                                "\"yes\"", "\"IMPOSSIBLE\"", "998244353",
                                "__builtin_popcount", "__builtin_popcountll", 
                                "mt19937_64", "mt19937"};

map<string, bool> specialPrefix = {
    {"mt19937", true}, 
    {"#define", true},
    {"const", true},
    {"typename", true},
    {"typedef", true},
    {"using", true},
    {"template", true},
    {"freopen", true},
    {"//", true},
    {"#pragma", true}
};

set<string> wordsInText;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);

    string testDirectory = "TestData";
    for (const auto & entry : fs::directory_iterator(testDirectory)) {
        ifstream inp(entry.path());

        int stringCount = 0;
        string line;
        while (inp >> line && stringCount <= 10) {
            if (line.size() > 50) {
                line = line.substr(0, 50);
            }
            if (line.size() >= 3) {
                int zeroCount = 0;
                bool isArrayLimit = false;

                // Check 100005, ... (array limit)
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] == '0') {
                        zeroCount++;
                        if (zeroCount >= 3) isArrayLimit = true;
                    }
                    else zeroCount = 0;
                }

                if (isArrayLimit) continue;

                //  Check number
                bool isNumber = true;
                for (int i = 0; i < line.size(); i++) {
                    if ('0' > line[i] || line[i] > '9') {
                        isNumber = false;
                    }
                }

                // Create a list contains strings appear in TestData
                if (!isNumber) {
                    //  Convert string s to "s"
                    string quotedLine = "\"" + line + "\"";
                    wordsInText.insert(quotedLine);

                    quotedLine = "\'" + line + "\'";
                    wordsInText.insert(quotedLine);
                }
                else {
                    wordsInText.insert(line);
                }

                stringCount++;
            }
        }
    }

    for (auto s: specialString) {
        if (wordsInText.find(s) != wordsInText.end()) {
            wordsInText.erase(s);
        }
    }

    vector<string> dictionary(wordsInText.begin(), wordsInText.end());

    vector<pair<long long, fs::path>> similarList;
    string submissionDirectory = "Submission";
    for (const auto & entry : fs::directory_iterator(submissionDirectory)) {
        ifstream inputFile(entry.path());

        long long similarity = 0;

        string line;
        string allStrings;
        while (getline(inputFile, line)) {
            string prefix;

            // Lines with define, const, ...
            for (auto ch: line) {
                if (ch != ' ') prefix += ch;
                else break;
            }

            if (specialPrefix.count(prefix)) continue;

            // Lines with array limit
            string newLine;
            stack<char> st;
            bool open = false, close = false;
            for (auto ch: line) {
                if (ch == '[') {
                    st.push(ch);
                }
                else if (ch == ']') {
                    if (st.size()) st.pop();
                }
                else {
                    if (st.empty()) newLine += ch;
                }
            }

            if (open && close) continue;

            allStrings += newLine + '\n';
        }

        similarity = countPattern(allStrings, dictionary);

        similarList.push_back({similarity, entry.path()});
    }

    sort(similarList.begin(), similarList.end(), greater<pair<long long, fs::path>>());

    for (auto [score, name]: similarList) {
        cout << score << ' ' << name << endl;
    }
 
    return 0;
}