from map import *

def get_total_operations() -> deque:
    raw_map = read_csv('python\\Route_Algorithm\\maze.csv')
    maze = Map(raw_map, 8, 6)
    endnode_num = len(maze.score_points)
    distance = [[0] * endnode_num] * endnode_num
    for i in range(endnode_num):
        for j in range(endnode_num):
            distance[i][j] = maze.get_distance(maze.score_points[i], maze.score_points[j])

if __name__ == "__main__":
    get_total_operations()