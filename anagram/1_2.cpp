#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include <fstream>
using namespace std;
int SCORES[26] = {1, 3, 2, 2, 1, 3, 3, 1, 1, 4, 4, 2, 2, 1, 1, 3, 4, 1, 1, 1, 2, 3, 3, 4, 3, 4};

vector<pair<string,string>> sort_dictionary(vector<string> dictionary){
    int dic_size = dictionary.size();
    vector<pair<string,string>> new_dictionary;
    for(string word : dictionary){
        string sorted_word = word;
        sort(sorted_word.begin(),sorted_word.end());
        new_dictionary.emplace_back(make_pair(sorted_word,word));
    }
    sort(new_dictionary.begin(),new_dictionary.end());
    return new_dictionary;
}
vector<pair<string,string>> new_dictionary;
//1つ目の宿題の関数　
string better_solution(string random_word,vector<pair<string,string>> new_dictionary){
    sort(random_word.begin(),random_word.end());
    auto bisearch = lower_bound(new_dictionary.begin(),new_dictionary.end(),make_pair(random_word,string("")));
    if(random_word==new_dictionary[bisearch-new_dictionary.begin()].first){
        return new_dictionary[bisearch-new_dictionary.begin()].second;
    }else return "";
}
vector<vector<int>> cnt(100100,vector<int>(26));
void count_characteres(const vector<string> dictionary,vector<vector<int>> cnt){
    int itr = 0;
    for(string word: dictionary){
        for(char letter:word){
            cnt[itr][letter]++;
            itr++;
        }
    }
    return;
}
string second_solution(string random_word, vector<string> dictionary, vector<vector<int>>cnt){
    string answer;
    vector<int> random_word_cnt(26,0);
    for(char c:random_word)random_word_cnt[c-'a']++;
    int max_score = 0,score = 0;
    for(int i = 0;i<dictionary.size();i++){
        for(int let = 0;let<26;let++){
            if(random_word_cnt[let]>cnt[i][let]){score = 0;break;}
            else score += SCORES[let];
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
    bool type;
    cin>>filetype>>type;
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
    ifs.close();
    if(type){
        //sort the dictionary
        new_dictionary = sort_dictionary(dictionary);
    }else count_characteres(dictionary,cnt);

    ofstream ofs(filetype + "_answer.txt");
    if(!ofs){
        cerr << "Error: answer file not opened." << endl;
        return 1;
    }
    //find an anagram with the higest score
    while(getline(ifs_input, str)){
        int mx_score = 0;
        string best_answer = "";
        cout<<str<<endl;
        if(type){
            for(int bit = 1;bit<(1<<str.length());bit++){
                string word="";
                for(int i = 0;i<str.length();i++){
                    if((bit>>i)&1)word += str[i];
                }
                string ans = better_solution(word,new_dictionary);
                if(ans!=""){
                    //cout<<ans<<endl;
                    int ret_score=0;
                    for(int i = 0;i < ans.length();i++)ret_score += SCORES[ans[i]-'a'];
                    if(mx_score<ret_score){
                        mx_score = ret_score;
                        best_answer = ans;
                    }
                }
            }
        }else{
            best_answer = second_solution(str);
        }
        ofs<<best_answer<<endl;
    }
    ifs_input.close();
    return 0;
}