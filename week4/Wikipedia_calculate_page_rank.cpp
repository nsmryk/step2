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
double small_number = 0.001;

struct PageRanking{
    std::string page_id="";
    double page_rank = 0.0;
    bool operator<( const PageRanking& right ) const {
        return page_rank == right.page_rank ? page_id < right.page_id : page_rank < right.page_rank;
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
// Sets initial value of PageRank 
std::vector<PageRanking> InitializePageRanking( std::map<std::string, std::string> pages,double score ) {
    std::vector<PageRanking> page_ranking;
    auto pages_begin = pages.begin(),pages_end = pages.end(); 
    for(auto iterator = pages_begin; iterator!=pages_end;iterator++){
        PageRanking page;
        page.page_id = iterator->first;
        page.page_rank = score;
        page_ranking.emplace_back(page);
    }
    return page_ranking;
}
// Maps page id to iterator of page_ranking
std::map<std::string, int> MapPageIdToIterator( std::vector<PageRanking> page_ranking ) {
    std::map<std::string, int> page_id_to_iterator;
    for(int iterator = 0;iterator < page_ranking.size(); iterator++ ){
        page_id_to_iterator[page_ranking[iterator].page_id] = iterator;
    }
    return page_id_to_iterator;
}
// Calculate score to give linked nodes and sum up the value for each node
std::vector<PageRanking> CalculatePageRank(std::vector<PageRanking> page_ranking, std::map<std::string, int> page_id_to_iterator, std::map<std::string, std::string> pages, std::map<std::string, std::set<std::string>> links) {   
    std::vector<PageRanking> next_score = InitializePageRanking(pages,0.0);
    for( int iterator = 0;iterator < page_ranking.size(); iterator++){
        double divided_score = page_ranking[iterator].page_rank/links[page_ranking[iterator].page_id].size();
        for( std::string linked_page_id : links[ page_ranking[iterator].page_id ] ) {
            next_score[page_id_to_iterator[ linked_page_id ] ].page_rank += divided_score;
        }
    }
    return next_score;
}
// Calculates absolute value of current_score - next_score and checks if the value is changed for each node
bool CheckIfScoreChanges(std::vector<PageRanking> page_ranking, std::vector<PageRanking> next_page_ranking) {
    bool is_any_score_changed = false;
    for( int iterator = 0;iterator < page_ranking.size(); iterator++){
        if( abs(page_ranking[iterator].page_rank -next_page_ranking[iterator].page_rank) > small_number ){
            is_any_score_changed = true;
        }
    }
    return is_any_score_changed;

}

// Outputs the shortest path from start_point to end_point and its length 
void OutputResult( std::vector<PageRanking> page_ranking, std::map<std::string, std::string> pages) {
  for(int rank = 0;rank<10;rank++){
        std::string page_id = page_ranking[rank].page_id;
        double page_rank_score = page_ranking[rank].page_rank;
        std::cout<<rank+1<<" : page name is "<<pages[page_id]<<" PageRank is "<<page_rank_score<<std::endl;
    }
    return;
}
int main() {
    std::map<std::string, std::string> pages = ReadPages();
    std::map<std::string, std::set<std::string>> links = ReadLinks();
    std::vector<PageRanking> page_ranking = InitializePageRanking(pages,first_score);
    std::map<std::string, int>  page_id_to_iterator = MapPageIdToIterator(page_ranking);
    
    
    bool is_any_score_changed = true;
    while( is_any_score_changed ){
        std::vector<PageRanking> next_page_ranking = CalculatePageRank(page_ranking, page_id_to_iterator, pages, links);
        is_any_score_changed = CheckIfScoreChanges( page_ranking , next_page_ranking);
        page_ranking = next_page_ranking;
    }
    
    std::sort(page_ranking.begin(),page_ranking.end());
    std::reverse(page_ranking.begin(),page_ranking.end());
    OutputResult(page_ranking,pages);
    

  return 0;
}