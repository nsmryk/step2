'''
    Opens file and reads coordinates
    Input filename
    Output cities : list of xy coordinates [(x_0,y_0),(x_1,y_1)...]
'''
def read_input(filename):
    with open(filename) as f:
        cities = []
        for line in f.readlines()[1:]:  # Ignore the first line.
            xy = line.split(',')
            cities.append((float(xy[0]), float(xy[1])))
        print("{0} is read".format(filename))
        #print(cities)
        return cities

'''
Formats output file
Input tour : cities lined up in the order of visit
Output : tour formatted for output file
'''
def format_tour(tour):
    return 'index\n' + '\n'.join(map(str, tour))

def print_tour(tour):
    print(format_tour(tour))
