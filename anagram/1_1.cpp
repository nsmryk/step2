#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include <fstream>
//#include<bits/stdc++.h>

using namespace std;
//bool operator< (const pair<string,string> a, const pair<string,string> b){return a.first < b.first;};

vector<string> better_solution(string random_word,vector<string> dictionary){
    sort(random_word.begin(),random_word.end());
    int dic_size = dictionary.size();
    vector<pair<string,string>> new_dictionary;
    for(string word : dictionary){
        string sorted_word = word;
        sort(sorted_word.begin(),sorted_word.end());
        new_dictionary.emplace_back(make_pair(sorted_word,word));
    }
    sort(new_dictionary.begin(),new_dictionary.end());
    auto bisearch = lower_bound(new_dictionary.begin(),new_dictionary.end(),make_pair(random_word,string("")));
    
    vector<string> anagram;
    while(random_word==new_dictionary[bisearch-new_dictionary.begin()].first){
        anagram.emplace_back(new_dictionary[bisearch-new_dictionary.begin()].second);
        bisearch++;
    }
    return anagram;
}
int main(){
    
    ifstream ifs("words.txt");
    if(!ifs){
        cerr << "Error: file not opened." << endl;
        return 1;
    }
    string str;
    vector<string> dictionary;
    while(getline(ifs, str)){
        dictionary.emplace_back(str);
    }
    ifs.close();
    string random_word;
    cin>>random_word;
    vector<string> answer;
    answer = better_solution(random_word,dictionary);
    for(string anagram: answer){
        cout<<anagram<<endl;
    }
    
    return 0;
}