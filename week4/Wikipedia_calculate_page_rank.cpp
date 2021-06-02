#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <algorithm>
double first_score = 100;
double small_number = 0.01;
struct PageRanking{
    std::string page_id="";
    double page_ranking = 0.0;
    bool operator<( const PageRanking& right ) const {
        return page_ranking == right.page_ranking ? page_id < right.page_id : page_ranking < right.page_ranking;
    }
};
// Reads pages.txt 
std::map<std::string, std::string> ReadPages(){
  std::ifstream file("data/pages.txt");
  std::string data;
  std::map<std::string, std::string> pages;
  while (std::getline(file, data)) {
    auto index = data.find('\t');
    auto id = data.substr(0, index);
    auto title = data.substr(index + 1, data.size() - id.size() - 1);
    pages[id] = title;
  }
  return pages;
}
// Reads links.txt 
std::map<std::string, std::set<std::string>> ReadLinks(){
  std::ifstream file("data/links.txt");
  std::string data;
  std::map<std::string, std::set<std::string>> links;
  while (std::getline(file, data)) {
    auto index = data.find('\t');
    auto from = data.substr(0, index);
    auto to = data.substr(index + 1, data.size() - from.size() - 1);
    links[from].insert(to);
  }
  return links;
}
std::vector<PageRanking> InitializePageRanking( std::map<std::string, std::string> pages ) {
    std::vector<PageRanking> page_ranking;
    auto pages_begin = pages.begin(),pages_end = pages.end(); 
    for(auto iterator = pages_begin; iterator!=pages_end;iterator++){
        PageRanking page;
        page.page_id = iterator->first;
        page.page_ranking = first_score;
        page_ranking.emplace_back(page);
    }
    return page_ranking;
}
std::map<std::string, int> MapPageIdToIterator( std::vector<PageRanking> page_ranking ) {
    std::map<std::string, int> page_id_to_iterator;
    for(int iterator = 0;iterator < page_ranking.size(); iterator++ ){
        page_id_to_iterator[page_ranking[iterator].page_id] = iterator;
    }
    return page_id_to_iterator;
}
std::vector<double> CalculatePageRank(std::vector<PageRanking> page_ranking, std::map<std::string, int> page_id_to_iterator, std::map<std::string, std::set<std::string>> links) {   
    std::vector<double> next_score(page_ranking.size(),0);
    for( int iterator = 0;iterator < page_ranking.size(); iterator++){
        double divided_score = page_ranking[iterator].page_ranking/links[page_ranking[iterator].page_id].size();
        for( std::string linked_page_id : links[ page_ranking[iterator].page_id ] ) {
            next_score[page_id_to_iterator[ linked_page_id ] ] += divided_score;
        }
    }
    return next_score;
}
bool CheckIfScoreChanges(std::vector<double> current_score, std::vector<double> next_score) {
    bool is_any_score_changed = false;
    for( int iterator = 0;iterator < current_score.size(); iterator++){
        if( abs(current_score[iterator]-next_score[iterator]) > small_number ){
            is_any_score_changed = true;
        }
    }
    return is_any_score_changed;

}

// Outputs the shortest path from start_point to end_point and its length 
void OutputResult( std::vector<PageRanking> page_ranking, std::map<std::string, std::string> pages) {
  for(int rank = 0;rank<10;rank++){
        std::string page_id = page_ranking[rank].page_id;
        double page_rank_score = page_ranking[rank].page_ranking;
        std::cout<<rank+1<<" : page name is "<<pages[page_id]<<" PageRank is "<<page_rank_score<<std::endl;
    }
    return;
}
int main() {
    std::map<std::string, std::string> pages = ReadPages();
    std::map<std::string, std::set<std::string>> links = ReadLinks();
    std::vector<PageRanking> page_ranking = InitializePageRanking(pages);
    std::map<std::string, int>  page_id_to_iterator = MapPageIdToIterator(page_ranking);
    
    
    bool is_any_score_changed = true;
    std::vector<double> current_score(page_ranking.size(),first_score);
    while( is_any_score_changed ){
        std::vector<double> next_score = CalculatePageRank(page_ranking,page_id_to_iterator, links);
        is_any_score_changed = CheckIfScoreChanges( current_score , next_score);
        current_score = next_score;
    }
    for(int i = 0;i < current_score.size();i++){
        page_ranking[i].page_ranking = current_score[i];
    }
    std::sort(page_ranking.begin(),page_ranking.end());
    std::reverse(page_ranking.begin(),page_ranking.end());
    OutputResult(page_ranking,pages);
    

  return 0;
}