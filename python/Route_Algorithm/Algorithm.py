from map import *

#Held-Karp Algorithm
def g(S:set, e:int) -> int:
    print(S, e)
    if len(S) == 0:
        return 0
    else:
        return min([g(S - {j}, j) + all_paths[j][e][2] for j in S])

def get_total_operations() -> int:
    raw_map = read_csv('python\\Route_Algorithm\\maze.csv')
    maze = Map(raw_map, 6, 8, starting_point_index = 1)
    SP_num = len(maze.score_points)
    
    global all_paths
    all_paths = [[0] * SP_num] * SP_num
    for i in range(SP_num):
        for j in range(SP_num):
            all_paths[i][j] = maze.path_find(maze.score_points[i].index, maze.score_points[j].index)
    
    #Held-Karp Algorithm
    S = set(range(SP_num))
    return g(S - {0}, 0)


if __name__ == "__main__":
    operations = get_total_operations()
    print(operations)