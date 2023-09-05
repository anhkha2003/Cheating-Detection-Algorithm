#include<bits/stdc++.h>
using namespace std;
 
namespace CountPattern {
    // to get all pattern ending at node:
    // node -> getLink(node) -> ... 
    // list all leaf at those nodes
    struct Vertex {
        static const int ALPHABET_SIZE = 128;
        
        int child[ALPHABET_SIZE], go[ALPHABET_SIZE];
        int idxLeaf = 0;
        int par = -1, link = -1;
        int nextLeaf = -1;
        char parChar; // edge par -> cur
     
        Vertex(int par = -1, char ch = 0) : par(par), parChar(ch) {
            memset(child, -1, sizeof(child));
            memset(go, -1, sizeof(go));
        }
    };
     
    struct Trie {
        static const char ALPHA = 1;
        vector<Vertex> node;
        Trie() {
            node.push_back({0}); 
        }
     
        // return leaf node
        int add(string s, int idx) {
            int cur = 0; // root = 0
            
            for (auto ch: s) {
                if (ch < ALPHA || ch >= Vertex::ALPHABET_SIZE) continue;

                int c = ch - ALPHA;
                if (node[cur].child[c] == -1) {
                    node[cur].child[c] = node.size();
                    node.push_back({cur, ch});
                }
                cur = node[cur].child[c];
            }
     
            node[cur].idxLeaf = idx;
            return cur;
        }
     
        int go(int cur, char ch) { // nextState[i][c]
            if (ch < ALPHA || ch >= Vertex::ALPHABET_SIZE) return 0;
            int c = ch - ALPHA;
            if (node[cur].go[c] == -1) {
                if (node[cur].child[c] != -1) {
                    node[cur].go[c] = node[cur].child[c];
                }
                else {
                    node[cur].go[c] = (cur == 0) ? 0 : go(getLink(cur), ch);
                }
            }
            return node[cur].go[c];
        }
     
        int getLink(int cur) {
            if (node[cur].link == -1) {
                if (!cur || !node[cur].par) {
                    node[cur].link = 0;
                }
                else {
                    node[cur].link = go(getLink(node[cur].par), node[cur].parChar);
                }
            }
            return node[cur].link;
        }
     
        int getNextLeaf(int cur) {
            if (cur == 0) return 0;
            if (node[cur].nextLeaf != -1) return node[cur].nextLeaf;
        
            int nxt = getLink(cur);
            if (node[nxt].idxLeaf) {
                return node[cur].nextLeaf = nxt;
            }       
        
            return node[cur].nextLeaf = getNextLeaf(nxt);
        }
    };
     
    long long countPattern(string &s, vector<string> &dict) {
        Trie aho;

        int k = dict.size();
        vector<long long> res(k + 1);
        vector<int> leaf(k + 1);

        int numChar = 0;
        for (int i = 1; i <= k; i++) {
            leaf[i] = aho.add(dict[i - 1], i);
            numChar += dict[i - 1].size();
        }

        vector<int> cnt(numChar + 1);

        int state = 0;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] <= 0 || s[i] >= Vertex::ALPHABET_SIZE) continue;
            state = aho.go(state, s[i]);
            cnt[state]++;
        }
        
        state = 0;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] <= 0 || s[i] >= Vertex::ALPHABET_SIZE) continue;
            state = aho.go(state, s[i]);
        
            if (cnt[state] == 0) continue;
        
            int cur = state;
            while (cur) {
                res[aho.node[cur].idxLeaf] += cnt[state];
                cur = aho.getNextLeaf(cur);
            }

            cnt[state] = 0;
        }
        
        long long ans = 0;
        for (int i = 1; i <= k; i++) {
            ans += res[aho.node[leaf[i]].idxLeaf];
        }

        return ans;
    }
}
