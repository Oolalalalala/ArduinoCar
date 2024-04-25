import csv
from collections import deque

class Map():
    def __init__(self, raw_map: list, x_width: int, y_width: int, starting_point_index: int):
        #Create nodes
        self.nodes = [Node(i, self) for i in range(1, x_width * y_width + 1)]
        self.starting_point = self.nodes[starting_point_index - 1]
        self.score_points = []
        
        #Add neighbor if there is one in the direction for each node  
        for i in range(x_width * y_width):  
            for j in range(1, 5):
                if not raw_map[i + 1][j] == '':
                    self.nodes[i].add_neighbor(self.nodes[int(raw_map[i + 1][j]) - 1], j - 1)
                    
        #Calculate score for each node and record score points
        for node in self.nodes:   
            if node.calculate_score() != 0:
                self.score_points.append(node)
    
    #Find the shortest path from A to B
    def path_find(self, A_index: int, B_index: int) -> tuple:
        #BFS while keeping track of all the subpaths
        searched = set()
        #Each node in the search_queue is a tuple (index to be searhed, distance from A)
        search_queue = deque([(A_index, 0)])
        #Each subpath is a tuple (index, next index, first node's distance from A)
        subpaths = deque()
        
        #Search until it's B's turn to be searched
        while search_queue[0][0] != B_index:
            searching, distance = search_queue.popleft()
            for neighbor in self.nodes[searching - 1].neighbors:
                
                #If the neighbor is not searched yet, record the subpath
                if neighbor is not None and neighbor.index not in searched:
                    subpaths.append((searching, neighbor.index, distance))
                    #Add the neighbor to the search_queue if it's not already in the queue
                    search_queue.append((neighbor.index, distance + 1)) if (neighbor.index, distance + 1) not in search_queue else None
                    
            #Add the searched node to the searched set after all its neighbors are searched
            searched.add(searching)
        
        if A_index != B_index:
            total_distance = distance
        else:
            total_distance = 0
        
        #Trace back all the shortest path by using all the subpaths
        paths = deque([deque([B_index])])
        while subpaths:
            this_subpath = subpaths.pop()
            path_distance = this_subpath[2]
            
            #Make sure all the paths have the correct length according to path_distance
            while len(paths[0]) + path_distance < total_distance:
                paths.popleft()
            
            #For each subpath, if the subpath fits the last node of the path, create a copy of the path and add the subpath to the path
            for i in range(len(paths)):
                if this_subpath[1] == paths[i][0]:
                    paths.append(paths[i].copy())
                    paths[-1].appendleft(this_subpath[0])
        
        #Remove all the paths that don't start with A
        while paths[0][0] != A_index:
            paths.popleft()
        
        starting = True if A_index == self.starting_point.index else False
        best_ETA = float('Inf')
        best_path = None
        best_operation = None
        
        for path in paths:
            operation = self.path_to_operation_basic(path, starting)
            if operation[1] < best_ETA:
                best_ETA = operation[1]
                best_path = path
                best_operation = operation
                
        return best_path, *best_operation
    
    def path_to_operation(self, path: deque, direction: int = None):
        direction = self.nodes[path[0] - 1].get_direction(self.nodes[path[1] - 1]) if direction is None else direction
        operations = ''
        
        #[0, 1, 2, 3] = [North, South, West, East]
        for i in range(len(path) - 1):
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
    
    def path_to_operation_basic(self, path: deque, starting: bool = False, direction: int = None) -> tuple:
        
        f_time = 2.4
        l_time = 0.38
        l2_time = 0.64
        r_time = 0.38
        i_time = 1.8
        
        if len(path) == 1:
            return ('', 0)
        
        def turn_left(dir):
            if dir == 0:
                return 2
            elif dir == 1:
                return 3
            elif dir == 2:
                return 1
            elif dir == 3:
                return 0
        
        direction = self.nodes[path[0] - 1].get_direction(self.nodes[path[1] - 1]) if direction is None else direction
        if not starting:
            direction = turn_left(turn_left(direction))        
        operations = 'f' if starting else ''
        ETA = 0
        
        #[0, 1, 2, 3] = [North, South, West, East]
        for i in range(1, len(path) - 1):
            next_direction = self.nodes[path[i] - 1].get_direction(self.nodes[path[i + 1] - 1])
            
            if direction == next_direction:
                operations += 'f'
                ETA += f_time
            elif (direction, next_direction) in [(0, 1), (1, 0), (2, 3), (3, 2)]:
                if self.nodes[path[i] - 1].neighbors[turn_left(direction)] is None:
                    operations += 'lf'
                else:
                    operations += 'llf'
                ETA += l2_time + f_time    
            elif (direction, next_direction) in [(0, 2), (2, 1), (1, 3), (3, 0)]:
                operations += 'lf'
                ETA += l_time + f_time
            elif (direction, next_direction) in [(0, 3), (3, 1), (1, 2), (2, 0)]:
                operations += 'rf'
                ETA += r_time + f_time
            direction = next_direction
        
        operations = operations[:-1] + 'i'
        ETA += i_time
        return (operations, ETA)
    
    #Print all nodes  
    def print(self):
        for node in self.nodes:   
            node.print()

class Node():
    def __init__(self, index: int, map: Map):
        self.map = map
        self.index = index
        self.x = (index - 1) // 6
        self.y = (index - 1) % 6
        self.score = None
        self.neighbors = [None, None, None, None]   #[North, South, West, East]
               
    def add_neighbor(self, neighbor, direction: int):
        self.neighbors[direction] = neighbor
    
    #Return the direction of the neighbor
    def get_direction(self, neighbor) -> int:
        for i in range(4):
            if self.neighbors[i] == neighbor:
                return i
            
    #If the node is a dead end (has only one neighbor), then the score is x + y
    def calculate_score(self) -> int:
        neighbor_count = 0
        for neighbor in self.neighbors:
            if neighbor is not None:
                neighbor_count += 1
        if neighbor_count == 1:
            self.score = abs(self.x - self.map.starting_point.x) + abs(self.y - self.map.starting_point.y)
            self.score *= 10
        else:
            self.score = 0
        return self.score
    
    #Format: Index:"index", x:"x", y:"y", score:"score", neighbors:[neighbor index list]
    def print(self):
        self.nei_list = [self.neighbors[i].index for i in range(4) if self.neighbors[i] is not None]
        print("Index:" + str(self.index) + ", x:" + str(self.x) + ", y:" + str(self.y) 
              + ", score:" + str(self.score) + ", neighbors:" + str(self.nei_list))


#Read csv file
def read_csv(file) -> list:
    with open(file, 'r') as f:
        reader = csv.reader(f, )
        data = list(reader)
    return data

def main():
    raw_map = read_csv('python\\Route_Algorithm\\maze.csv')
    maze = Map(raw_map, 8, 6, starting_point_index = 1)
    
    # Enter the start and end point here
    best_path = maze.path_find(1, 1)
    print(best_path)
    
    
if __name__ == "__main__":
    main()