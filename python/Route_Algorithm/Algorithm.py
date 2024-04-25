import math
try:
    from .map import *
except ImportError:
    from map import *

calculated = False

def to_power(i: int) -> set:
    result = set()
    for j in range(math.ceil(math.log2(i)) + 1):
        if i & (1 << j) == 1 << j:
            result.add(j)
    return result

def calcultate_path():
    raw_map = read_csv('python\\Route_Algorithm\\big_maze_112.csv')
    maze = Map(raw_map, 6, 8, starting_point_index = 6)
    SP_num = len(maze.score_points)

    #Held-Karp Algorithm
    paths_from_start = [maze.path_find(maze.starting_point.index, maze.score_points[i].index) for i in range(SP_num)]
    all_paths = [[maze.path_find(maze.score_points[i].index, maze.score_points[j].index) for j in range(SP_num)] for i in range(SP_num)]
    
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

def get_total_operations():
    if not calculated:
        calcultate_path()
    return best_total_operations

def get_best_route():
    if not calculated:
        calcultate_path()
    return best_route

def get_best_ETA_total():
    if not calculated:
        calcultate_path()
    return best_ETA_total


if __name__ == "__main__":
    operations = get_total_operations()
    route = get_best_route()
    ETA = get_best_ETA_total()
    print(operations)
    print(route)
    print(ETA)