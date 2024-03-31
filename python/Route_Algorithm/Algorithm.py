import csv

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
    raw_map = read_csv('python\Route_Algorithm\maze.csv')
    maze = Map(raw_map, 8, 6)
    maze.print()

if __name__ == "__main__":
    main()