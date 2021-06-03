#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>

// Reads pages.txt 
std::map<std::string, std::string> ReadPages(){
  std::ifstream file("data/pages.txt");
  if( !file ){
    std::cout<<"Error: There's no such file"<<std::endl;
  }
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
  if( !file ){
    std::cout<<"Error: There's no such file"<<std::endl;
  }
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
// Finds start point (Google) or end point (Shibuya) and return its id 
std::string FindTargetNode( std::string target_name, const std::map<std::string, std::string> pages) {
  std::string target_id;
  for ( const auto& page : pages ) {
    if ( page.second == target_name ) {
      target_id = page.first;
      break;
    }
  }
  return target_id;
}
// BFS using queue, and records the path from start_point_id
std::map<std::string, std::string> FindShortestPathByBFS(std::string start_point_id, std::string end_point_id, std::map<std::string, std::set<std::string>> links) {
  std::queue<std::string> nodes_for_search;
  nodes_for_search.push(start_point_id);
  std::map<std::string, std::string> record_previous_node;
  bool is_path_found;
  while( !nodes_for_search.empty() ){
    std::string current_id = nodes_for_search.front();
    nodes_for_search.pop();
    for(std::string next_id : links[current_id]){
      if ( record_previous_node[next_id] == "" ) { //Checks if next_id is already looked
        record_previous_node[next_id] = current_id;
        if( next_id == end_point_id ){
          is_path_found  = true;
          break;
        }else {
          nodes_for_search.push(next_id);
        }
      }
    }
  }
  if( !is_path_found ){
    record_previous_node[start_point_id] = -1;
  } 
  return record_previous_node;
}
// Follows the path from start_point_id to end_point_id and store it in array
std::vector<std::string> TranslateMapToVector(std::string start_point_id, std::string end_point_id, std::map<std::string, std::string> pages, std::map<std::string, std::string> record_previous_node) {
  std::vector<std::string> shortest_path;
  std::string current_id = end_point_id;
  while ( current_id != start_point_id ) {
    shortest_path.emplace_back(pages[current_id]);
    current_id = record_previous_node[current_id];
  }
  shortest_path.emplace_back(pages[start_point_id]);
  return shortest_path;
}
// Outputs the shortest path from start_point to end_point and its length 
void OutputResult( std::string start_point_id, std::string end_point_id, std::map<std::string, std::string> pages, std::vector<std::string> shortest_path) {
  
  std::cout<<"The shortest path is as follows"<<std::endl;
  
  for(int i = shortest_path.size()-1;i>=0;i--){
    std::cout<<shortest_path[i];
    if( i > 0 )std::cout<<"->";
    else std::cout<<std::endl;
  }
  int distance = shortest_path.size() - 1;
  std::cout<<"the distance between "<<pages[start_point_id]<<" and "<<pages[end_point_id]<<" is "<<distance<<std::endl;
  return;
}
int main() {
  std::map<std::string, std::string> pages = ReadPages();
  std::map<std::string, std::set<std::string>> links = ReadLinks();
  
  std::string start_point_id,end_point_id;
  start_point_id = FindTargetNode( "Google" , pages);
  end_point_id = FindTargetNode( "渋谷" , pages );
  
  std::map<std::string, std::string> record_previous_node = FindShortestPathByBFS(start_point_id, end_point_id, links);//id -> previous page's id  

  if( record_previous_node[start_point_id] == "-1" ){
    std::cout<< "There is no way to "<<pages[end_point_id]<<std::endl;
  }else {
    std::vector<std::string> shortest_path = TranslateMapToVector(start_point_id, end_point_id, pages, record_previous_node);
    OutputResult(start_point_id,end_point_id,pages,shortest_path);
  }
  return 0;
}