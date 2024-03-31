import csv
from collections import deque

class Map():
    def __init__(self, raw_map, x_width, y_width):
        #Create nodes
        self.nodes = [Node(i, self) for i in range(1, x_width * y_width + 1)]
        
        #Add neighbor if there is one in the direction for each node  
        for i in range(x_width * y_width):  
            for j in range(1, 5):
                if not raw_map[i + 1][j] == '':
                    self.nodes[i].add_neighbor(self.nodes[int(raw_map[i + 1][j]) - 1], j - 1)
                    
        #Calculate score for each node
        for node in self.nodes:   
            node.calculate_score()
    
    #Find the shortest path from A to B
    def path_find(self, A_index, B_index):
        #BFS while keeping track of all the subpaths
        searched = set()
        subpaths = deque()
        search_queue = deque([(A_index, 0)])
        
        while search_queue[0][0] != B_index:
            searching, distance = search_queue.popleft()
            for neighbor in self.nodes[searching - 1].neighbors:
                if neighbor is not None and neighbor.index not in searched:
                    subpaths.append((searching, neighbor.index, distance))
                    search_queue.append((neighbor.index, distance + 1)) if (neighbor.index, distance + 1) not in search_queue else None
            searched.add(searching)
        total_distance = distance
        
        #Trace back all the shortest path
        paths = deque([deque([B_index])])
        while subpaths:
            this_subpath = subpaths.pop()
            path_distance = this_subpath[2]
            while len(paths[0]) + path_distance < total_distance:
                paths.popleft()
            for i in range(len(paths)):
                if this_subpath[1] == paths[i][0]:
                    paths.append(paths[i].copy())
                    paths[-1].appendleft(this_subpath[0])
        while paths[0][0] != A_index:
            paths.popleft()
                            
        return paths
    
    def path_to_operation(self, path):
        operations = 'f'
        direction = self.nodes[path[0] - 1].get_direction(self.nodes[path[1] - 1])
        for i in range(1, len(path) - 1):
            next_direction = self.nodes[path[i] - 1].get_direction(self.nodes[path[i + 1] - 1])
            if direction == next_direction:
                operations += 'f'
            elif (direction, next_direction) in [(0, 1), (1, 0), (2, 3), (3, 2)]:
                operations += 'b'
            elif (direction, next_direction) in [(0, 2), (2, 1), (1, 3), (3, 0)]:
                operations += 'l'
            elif (direction, next_direction) in [(0, 3), (3, 1), (1, 2), (2, 0)]:
                operations += 'r'
            direction = next_direction
        return operations
    
    #Print all nodes  
    def print(self):
        for node in self.nodes:   
            node.print()

class Node():
    def __init__(self, index, map):
        self.map = map
        self.index = index
        self.x = (index - 1) // 6
        self.y = (index - 1) % 6
        self.score = None
        self.neighbors = [None, None, None, None]   #[North, South, West, East]
               
    def add_neighbor(self, neighbor, direction):
        self.neighbors[direction] = neighbor
    
    def get_direction(self, neighbor):
        for i in range(4):
            if self.neighbors[i] == neighbor:
                return i
            
    #If the node is a dead end (has only one neighbor), then the score is x + y
    def calculate_score(self):
        neighbor_count = 0
        for neighbor in self.neighbors:
            if neighbor is not None:
                neighbor_count += 1
        if neighbor_count == 1:
            self.score = self.x + self.y
        else:
            self.score = 0
    
    #Format: Index:"index", x:"x", y:"y", score:"score", neighbors:[neighbor index list]
    def print(self):
        self.nei_list = [self.neighbors[i].index for i in range(4) if self.neighbors[i] is not None]
        print("Index:" + str(self.index) + ", x:" + str(self.x) + ", y:" + str(self.y) 
              + ", score:" + str(self.score) + ", neighbors:" + str(self.nei_list))


#Read csv file
def read_csv(file):
    with open(file, 'r') as f:
        reader = csv.reader(f, )
        data = list(reader)
    return data

def main():
    raw_map = read_csv('python\\Route_Algorithm\\maze.csv')
    maze = Map(raw_map, 8, 6)
    
    # Enter the start and end point here
    paths = maze.path_find(1, 48)
    for path in paths:
        print(path)
        print(maze.path_to_operation(path))

if __name__ == "__main__":
    main()