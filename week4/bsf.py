from queue import Queue


def get_id(page_name, pages):
    for key, name in pages.items():
        if name == page_name:
            return key
    return -1


def path_to_string(path, pages):
    if (path == ''):
        return 'no path found'
    result = ''
    for node_id in path:
        result += f'{pages[node_id]} ({node_id}) -> '

    # Cut off the ' -> ' at the end.
    return result[:-4]


def find_shortest_path(start_name, finish_name, pages, links):
    # Find ids of start and finish. Return invalid if pages aren't known.
    start_id = get_id(start_name, pages)
    if start_id == -1:
        return []

    finish_id = get_id(finish_name, pages)
    if finish_id == -1:
        return []

    # Find the shortest path. Queue will store paths to the current node.
    search_queue = Queue()
    search_queue.put({'id': start_id, 'path': [start_id]})
    # Visited will mark the nodes that are already visited.
    visited = set()
    
    while not search_queue.empty():
        node = search_queue.get()
        print(type(links[node['id']]))
        # If the current element is the target, return the path to it.
        if node['id'] == finish_id:
            return node['path']

        # Set that we've already visited this element.
        visited.add(node['id'])

        # Continue to the next node if this node has no linked nodes.
        if node['id'] not in links:
            continue

        # Push all of the linked nodes of the current node into the queue.
        for linked_id in links[node['id']]:
            # If we've already visited the node, skip it.
            if linked_id in visited:
                continue

            linked_path = node['path'].copy()
            linked_path.append(linked_id)

            search_queue.put({'path': linked_path, 'id': linked_id})

    return []


def print_shortest_path(start, finish, pages, links):
    path = find_shortest_path(start, finish, pages, links)
    path_string = path_to_string(path, pages)
    print(f'from {start} to {finish}: {path_string}')


def main_small():
    pages = {}
    links = {}

    with open('data/pages_small.txt') as f:
        for data in f.read().splitlines():
            page = data.split('\t')
            # page[0]: id, page[1]: title
            pages[int(page[0])] = page[1]

    with open('data/links_small.txt') as f:
        for data in f.read().splitlines():
            link = data.split('\t')

            # link[0]: id (from), links[1]: id (to)
            id_from = int(link[0])
            id_to = int(link[1])
            if id_from in links:
                links[id_from].add(id_to)
            else:
                links[id_from] = {id_to}

    print_shortest_path('Google', 'Google', pages, links)
    print_shortest_path('Google', '健康', pages, links)
    print_shortest_path('Google', '環境', pages, links)


def main():
    pages = {}
    links = {}

    with open('data/pages.txt') as f:
        for data in f.read().splitlines():
            page = data.split('\t')
            # page[0]: id, page[1]: title
            pages[int(page[0])] = page[1]

    with open('data/links.txt') as f:
        for data in f.read().splitlines():
            link = data.split('\t')

            # link[0]: id (from), links[1]: id (to)
            id_from = int(link[0])
            id_to = int(link[1])
            if id_from in links:
                links[id_from].add(id_to)
            else:
                links[id_from] = {id_to}

    # Google (457783) -> セグウェイ (154009) -> 渋谷 (22557)
    # print_shortest_path('Google', '渋谷', pages, links)

    # サマルカンド州 (501160) -> 世界遺産 (8732) -> 筑波大学 (4196)
    print_shortest_path('Google', 'Google', pages, links)


if __name__ == '__main__':
    main_small()