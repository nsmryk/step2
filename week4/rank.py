def ReadPages(pages):
    with open('data/pages_small.txt') as f:
        for data in f.read().splitlines():
            page = data.split('\t')
            # page[0]: id, page[1]: title
            pages[page[0]] = page[1]


def ReadLinks(links):
    with open('data/links_small.txt') as f:
        for data in f.read().splitlines():
            link = data.split('\t')
            # link[0]: id (from), links[1]: id (to)
            if link[0] in links:
                links[link[0]].add(link[1])
            else:
                links[link[0]] = {link[1]}


def isInt(s):
    try:
        int(s, 10)
    except ValueError:
        return False
    else:
        return True


def AddScore(pages, links, score_list):
    new_score_list = {}
    for id in links:
        # Add equal score
        add_score = score_list[id] / len(links[id])
        for each_id in links[id]:
            if each_id in new_score_list:
                new_score_list[each_id] += add_score
            else:
                new_score_list[each_id] = add_score
    # Titles that no one have their link have score 0
    for id in pages:
        if id not in new_score_list:
            new_score_list[id] = 0
    return new_score_list


def InitScoreList(score_list, pages, init_score):
    for id in pages:
        score_list[id] = init_score


def PageRank(pages, links, times, init_score):
    # Set initial value
    score_list = {}
    InitScoreList(score_list, pages, init_score)

    # Calculation
    for count in range(times):
        score_list = AddScore(pages, links, score_list)
    return score_list


def PrintPageRank(result, pages, output_count):
    result_sorted = sorted(result.items(), key=lambda x: x[1], reverse=True)
    if output_count == 'ALL':
        output_count = len(result_sorted)
    elif isInt(output_count):
        output_count = int(output_count)
    else:
        print("Please Enter the Correct Value")
        exit(1)

    # Output
    print("\nRanking, ID, Title, Score")
    count = 1
    for id, score in result_sorted:
        print('{0}, {1}, {2}, {3:.3f}, '.format(
            count, id, pages[id], score))
        if count == output_count:
            break
        count += 1


def main():
    # User Input
    print('Times > ', end="")
    times = int(input())
    print('Initial Score > ', end="")
    init_score = int(input())
    print('Number of outputs > ', end="")
    output_count = input()

    # Read Data
    pages = {}
    links = {}
    ReadPages(pages)
    ReadLinks(links)

    # Output Result
    result = PageRank(pages, links, times, init_score)
    PrintPageRank(result, pages, output_count)


if __name__ == '__main__':
    main()