#include<bits/stdc++.h>
#include "finding_pattern.h"
using namespace std;
using namespace CountPattern;
namespace fs = std::filesystem;

string special[3] = {"YES", "Yes", "IMPOSSIBLE"};
set<string> wordsInTest;

long long calcMagic(string &s) {
    vector<string> dict(wordsInTest.begin(), wordsInTest.end());
    return countPattern(s, dict);
}

int main() {
    freopen("output.txt", "w", stdout);
    ios::sync_with_stdio(0);
    cin.tie(NULL);

    string pathTest = "TestData";
    for (const auto & entry : fs::directory_iterator(pathTest)) {
        ifstream inp(entry.path());

        int cntString = 0;
        string s;
        while (inp >> s && cntString <= 10) {
            cntString++;
            if (s.size() > 50) {
                s = s.substr(0, 50);
            }
            if (s.size() >= 3) {
                int cnt0 = 0;
                bool notLimit = 1;

                // Check 100005, ... (array limit)
                for (int i = 0; i < s.size(); i++) {
                    if (s[i] == '0') {
                        cnt0++;
                        if (cnt0 >= 2) notLimit = 0;
                    }
                    else cnt0 = 0;
                }

                //  Check number
                bool isNum = 1;
                for (int i = 0; i < s.size(); i++) {
                    if ('0' > s[i] || s[i] > '9') {
                        isNum = 0;
                    }
                }

                // Create a list contains strings appear in TestData
                if (notLimit) {
                    if (!isNum) {
                        //  Convert string s to "s"
                        string ss = "\"" + s + "\"";
                        wordsInTest.insert(ss);

                        ss = "\'" + s + "\'";
                        wordsInTest.insert(ss);
                    }
                    else {
                        wordsInTest.insert(s);
                    }
                }

                cntString++;
            }
        }
    }

    for (auto s: special) {
        wordsInTest.erase(s);
    }

    // for (auto s: magic) {
    //     cout << s.first << ' ' << s.second << endl;
    // }

    string path = "Submission";
    for (const auto & entry : fs::directory_iterator(path)) {
        ifstream inp(entry.path());
        cout << entry.path() << ' ';

        long long numMagic = 0;

        string s;
        vector<pair<string, string>> magic;

        while (getline(inp, s)) {
            string prefix;
            for (auto i: s) {
                if (i != ' ') prefix += i;
                else break;
            }
            if (prefix == "#define" || prefix == "const" || prefix == "typename") {
                continue;
            }

            numMagic += calcMagic(s);
        }

        if (numMagic >= 3) cout << "If Test" << endl;
        else cout << "Valid" << endl;
        // string filename = entry.path().filename();
        // if (magic.size() >= 2) cout << filename << '\n';
    }
 
    return 0;
}