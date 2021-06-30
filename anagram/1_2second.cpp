#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include <fstream>
using namespace std;
int SCORES[26] = {1, 3, 2, 2, 1, 3, 3, 1, 1, 4, 4, 2, 2, 1, 1, 3, 4, 1, 1, 1, 2, 3, 3, 4, 3, 4};
vector<vector<int>> cnt(100100,vector<int>(26));

string second_solution(string random_word, vector<string> dictionary){
    string answer;
    vector<int> random_word_cnt(26,0);
    for(int let = 0;let<random_word.length();let++){
        random_word_cnt[random_word[let]-'a']++;
    }
    int max_score = 0,score = 0;
    for(int i = 0;i<dictionary.size();i++){
        score = 0;
        
        for(int let = 0;let<26;let++){
            if(random_word_cnt[let]< cnt[i][let]){score = 0;break;}
            else score += SCORES[let]*cnt[i][let];
        }
        if(score>max_score){
            answer = dictionary[i];
            max_score = score;
        }
    }
    return answer;
}
int main(){
    string filetype;
    cin>>filetype;

    //ファイル読み込み
    ifstream ifs_input(filetype+".txt"),ifs("words.txt");
    if(!ifs){
        cerr << "Error: dictionary file not opened." << endl;
        return 1;
    }
    string str;
    vector<string> dictionary;
    while(getline(ifs, str)){
        dictionary.emplace_back(str);
    }
    
    vector<pair<string,string>> new_dictionary;
    
    
    for(int i = 0;i<dictionary.size();i++){
        string word = dictionary[i];
        for(int let = 0;let<word.length();let++){
            cnt[i][word[let]-'a']++;
        }
    }
        
    //出力ファイル準備
    ofstream ofs(filetype + "_answer.txt");
    if(!ofs){
        cerr << "Error: answer file not opened." << endl;
        return 1;
    }
   
    while(getline(ifs_input, str)){
        int mx_score = 0;
        string best_answer = "";
        best_answer = second_solution(str,dictionary);
        
        ofs<<best_answer<<endl;
    }
    ifs.close();
    ifs_input.close();
    ofs.close();
    return 0;
}