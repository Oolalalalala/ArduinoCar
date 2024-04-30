import math
try:
    from .map import *
except ImportError:
    from map import *

time_limit = 80
calculated = False
limited_calculated = False

def to_power(i: int) -> set:
    result = set()
    for j in range(math.ceil(math.log2(i)) + 1):
        if i & (1 << j) == 1 << j:
            result.add(j)
    return result

def calcultate():
    raw_map = read_csv('python\\Route_Algorithm\\maze.csv')
    
    global maze, SP_num
    maze = Map(raw_map, 3, 2, starting_point_index = 4)
    SP_num = len(maze.score_points)

    #Held-Karp Algorithm
    global paths_from_start, all_paths
    paths_from_start = [maze.path_find(maze.starting_point.index, maze.score_points[i].index) for i in range(SP_num)]
    all_paths = [[maze.path_find(maze.score_points[i].index, maze.score_points[j].index) for j in range(SP_num)] for i in range(SP_num)]
    
    global g
    g = [[([i], paths_from_start[i][2]) for i in range(SP_num)]] + [[] for _ in range(2**SP_num - 2)]
    
    for i in range(1, 2**SP_num - 1):
        subset_i = to_power(i)
        for j in range(SP_num):
            if j not in subset_i:
                best_node = None
                best_ETA = float('inf')
                for k in subset_i:
                    ETA_k = g[i - (1 << k)][k][1] + all_paths[k][j][2]
                    if ETA_k < best_ETA:
                        best_node = k
                        best_ETA = ETA_k
                g[i].append((g[i - (1 << best_node)][best_node][0] + [j], best_ETA))
            else:
                g[i].append(None)
    #Algorithm end
    
    global best_route, best_total_operations, best_ETA_total, calculated
    
    best_end = None
    best_ETA_total = float('inf')     
    for e in range(SP_num):
        ETA_total_e = g[2**SP_num - 1 - (1 << e)][e][1]
        if ETA_total_e < best_ETA_total:
            best_end = e
            best_ETA_total = ETA_total_e
    
    
    best_route = g[2**SP_num - 1 - (1 << best_end)][best_end][0]
    best_total_operations = deque(paths_from_start[best_route[0]][1])
    for i in range(1, SP_num):
        best_total_operations += deque(all_paths[best_route[i - 1]][best_route[i]][1])
        
    calculated = True

def calculate_limited():
    if not calculated:
        calcultate()
    
    global best_limited_route, best_limited_score
    best_limited_route = None
    best_limited_score = 0
    for i in range(2**SP_num - 1):
        for route in g[i]:
            if route is not None and route[1] <= time_limit:
                route_score = sum([maze.score_points[j].score for j in route[0]])
                if route_score > best_limited_score:
                    best_limited_route = route
                    best_limited_score = route_score
                elif route_score == best_limited_score and route[1] < best_limited_route[1]:
                    best_limited_route = route
                    best_limited_score = route_score
    
    global best_limited_operations
    best_limited_operations = deque(paths_from_start[best_limited_route[0][0]][1])
    for i in range(1, len(best_limited_route[0])):
        best_limited_operations += deque(all_paths[best_limited_route[0][i - 1]][best_limited_route[0][i]][1])
        
    limited_calculated = True

def get_total_operations():
    if not calculated:
        calcultate()
    return best_total_operations

def get_best_route():
    if not calculated:
        calcultate()
    return best_route

def get_best_ETA_total():
    if not calculated:
        calcultate()
    return best_ETA_total

def get_best_score():
    return sum([point.score for point in maze.score_points])

def get_limited_operations():
    if not limited_calculated:
        calculate_limited()
    return best_limited_operations

def get_limit_route():
    if not limited_calculated:
        calculate_limited()
    return best_limited_route[0]

def get_limited_ETA_total():
    if not limited_calculated:
        calculate_limited()
    return best_limited_route[1]

def get_limited_score():
    if not limited_calculated:
        calculate_limited()
    return best_limited_score


if __name__ == "__main__":
    
    total_operations = get_total_operations()
    best_route = get_best_route()
    best_ETA_total = get_best_ETA_total()
    total_score = get_best_score()
    
    print(total_operations)
    print(best_route)
    print(best_ETA_total)
    print(total_score)
    
    print()
    
    limited_operations = get_limited_operations()
    limited_route = get_limit_route()
    limited_ETA_total = get_limited_ETA_total()
    limited_score = get_limited_score()
    
    print(limited_operations)
    print(limited_route)
    print(limited_ETA_total)
    print(limited_score)
    