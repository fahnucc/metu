#include "the8.h"

//DO NOT ADD OTHER LIBRARIES

using namespace std;
long hashy(const string& str)
{
    long hash = 5381;
    for (size_t i = 0; i < str.size(); ++i)
        hash = 33 * hash + (unsigned char)str[i];
    
    return hash;
}


vector<int> the8(const string& text, const vector<string>& patterns){

    vector<int> shifts; //DO NOT CHANGE THIS

    long pSize = patterns.size();
    long p[pSize] = {0};
    long M = patterns[0].length();
    long N = text.length();
    int i, k;
    long t = 0;
    long h = 1;
    long d = 26;
    
    long q = 1000003;
    if(pSize < 10000) q = 100003;
    
    vector<vector<string>> hashTable;
    
    for(i = 0; i < q; i++){
        vector<string> dummy;
        hashTable.push_back(dummy);
    }
 
    for (i = 0; i < M - 1; i++)
    {
        h = (h * d) % q;
    }
        
    for (i = 0; i < M; i++)
    {
        for(k = 0; k < pSize; k++)
        {
            p[k] = (d * p[k] + patterns[k][i]) % q;
            if(p[k] < 0) p[k] += q;
            hashTable[p[k]].push_back(patterns[k]);
        }
        t = (d * t + text[i]) % q;
        if(t < 0) t += q;
    }
    
    for (i = 0; i <= N - M; i++)
    {
        for(k = 0; k < hashTable[t].size(); k++){
            if(text.compare(i, M, hashTable[t][k]) == 0)
            {
                shifts.push_back(i);
            }
        }
            
        if (i < N - M)
        {
            t = (d*(t - text[i]*h) + text[i+M]) % q;
            if (t < 0) t += q;

        }
    }

    return shifts; //DO NOT CHANGE THIS
}