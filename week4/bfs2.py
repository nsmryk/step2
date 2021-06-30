word = input().split()

start_word = word[0]
goal_word = word[1]


def word_search(path_record, link, pages, links, que, visited, present_number, goal_number, path_length):
  print(type(present_number))
  present = str(present_number)
  if link[present_number] == 1:
    for next_number in links[present]:
    
       if visited[int(next_number)] == 0:
         que.append(next_number)
        
         for i in range(path_length + 1):
           path_record[int(next_number)][i+1] = present_number
           
           present_number = path_record[present_number][1]
         print("{} > {}".format(pages[present], pages[next_number]))
         
         visited[int(next_number)] = 1
        
       else:
         continue
         
        
  

def que_change(path_record, link, pages, links, que, visited, goal_number, path_length):
  que_length = len(que)
  i = 0
  
  while i < que_length:
    
    if que[0] == goal_number:
      break
    word_search(path_record, link, pages, links, que, visited, que[0], goal_number, path_length)
    del que[0]
    i += 1


  if i < que_length:
    return goal_number

  elif len(que) == 0:
    return -1
  else:
    return 0
  
      

def bfs(path_record, link, pages, links, start, goal):
  visited = []
  que = []
  path_length = 0
  goal_number = -1
  start_exist = 0
  goal_exist = 0
  the_shortest_path = 0
  N = 1483277
  
  for i in range(N):
    visited.append(0)

  for word_number, word in pages.items():
    
    if word == start:
      start_exist = 1
      visited[int(word_number)] = 1
      que.append(word_number)
    if word == goal:
      goal_exist = goal
      goal_number = word_number
  
  if start_exist == 0 or goal_exist == 0:
    print("Error:{} or {} don't exist on Wikipedia.".format(start, goal))
    return -1, -1
  
  elif start == goal:
    return 0, goal_number
  
  else: 
    while True:
      n = que_change(path_record, link, pages, links, que, visited, goal_number, path_length)
  
      loop_end = (str(n) == goal_number)
      if loop_end == True:
        break

      elif n == -1:
        path_length = -10
        break

      path_length += 1

      print("path_length:{}".format(path_length))
  
    return path_length, goal_number

      

def main():
  N = 1483277 #len(pages)
  pages = {}
  links = {}
  path_record = [[0]*100 for i in range(N)]

  with open('data/pages_small.txt') as f:
    for data in f.read().splitlines():
      page = data.split('\t')
      # page[0]: id, page[1]: title
      
      pages[page[0]] = page[1]
      vertex = page[0]
      
      path_record[int(vertex)][0] = vertex
  
  link_to_next = []
 
  for i in range(N):
    link_to_next.append(0)

  with open('data/links_small.txt') as f:
    for data in f.read().splitlines():
      link = data.split('\t')
      # link[0]: id (from), links[1]: id (to)
      if link[0] in links:
        
        links[link[0]].add(link[1])
      else:
        links[link[0]] = {link[1]}
        vertex = link[0]
        
        link_to_next[int(vertex)] = 1
    
  
  path_length, goal_number = bfs(path_record, link_to_next, pages, links, start_word, goal_word)
  
  if path_length < 0:
    print("{} and {} are not connected on Wikipedia.".format(start_word, goal_word))
  else:
    print("the smallest number of edge from {} to {} is {}.".format(start_word, goal_word, path_length))
    print("path:")
    for i in range(path_length+1):
      print(pages[str(path_record[int(goal_number)][path_length-i])])

    


if __name__ == '__main__':
  main()