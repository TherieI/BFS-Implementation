## Learning C++ with Breadth First Search

In order to grasp the basic concepts of C++ in an engaging way, I decided to work on a mini-project involving `Breadth First Search`, an algorithm we covered in class. I used `BFS` to solve a randomly generated maze. The *start* position is generated *randomly* and the goal is to reach the bottom level of the maze.

**Cell Struct**

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>

struct Cell {
    enum Type {
            WALL,
            SEARCH,
            START,
            GOAL,
            EMPTY
    };
    int x;
    int y;
    Type type;
    bool visited;

    Cell () {  // Default Cell (Out of bounds)
        this->x = -1;
        this->y = -1;
        this->type = EMPTY;
        this->visited = false;
    }

    Cell (int x, int y, Type type) {
        this->x = x;
        this->y = y;
        this->type = type;
        this->visited = false;
    }

    void setType(Type type) {
        this->type = type;
    }
    Type getType() {
        return type;
    }

    void see() {
        this->visited = true;
    }
};
```

We have a grid of `N × N` cells, each cell storing a state of the maze. For example, `Wall` cells are marked with the *WALL* type, `Empty` cells are *EMPTY*.

**Grid class** (BFS Methods)

```cpp
class Grid {
private:
    int width;
    int height;
    Cell grid[100][100];
    Cell outOfBoundsCell = Cell();
    int randBuffer = time(0);
    std::map<Cell*, Cell*> paths;

public:
    Grid(int width, int height) {
        if (width > 100) {
            width = 100;
        }
        if (height > 100) {
            height = 100;
        }
        if (width < 9) {
            width = 9;
        }
        if (height < 9) {
            height = 9;
        }
        this->width = width;
        this->height = height;
    }

    Cell* getCell(int x, int y) {
        if (inGridBounds(x, y)) {
            return &grid[x][y];
        }
        return &outOfBoundsCell;
    }
    
    void setCell(int x, int y, Cell::Type type) {
        if (inGridBounds(x, y)) {
            grid[x][y].setType(type);
        }
    }
    
    /* * Omitted methods * */
    
}
```

At its core, the grid class is a depiction of the maze, based on the type of each cell. You may notice that each method accessing a cell always returns some form of `Cell *` (a pointer to a cell). This is so we can directly access each cell in the grid instead of getting a copy.

**Grid class** (Creating the Maze)

```cpp
void create() {
    srand(randBuffer++);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (y == 0 || y == height - 1 || x == 0 ) {
                grid[x][y] = Cell(x, y, Cell::Type::WALL);
            } else if (x == width - 1) {
                grid[x][y] = Cell(x, y, Cell::Type::GOAL);
            } else if (x % 2 == 0 && y % 2 == 0) {
                grid[x][y] = Cell(x, y, Cell::Type::WALL);
            } else if ((x % 2 == 1 && y % 2 == 0) || (x % 2 == 0 && y % 2 == 1)) {
                float randnum = (float) rand() / RAND_MAX;
                if (randnum > 0.6) {
                    grid[x][y] = Cell(x, y, Cell::Type::WALL);
                } else {
                    grid[x][y] = Cell(x, y, Cell::Type::EMPTY);
                }
            }
            else {
                grid[x][y] = Cell(x, y, Cell::Type::EMPTY);
            }
        }
    }
    // Entrance and Exit
    int randSpawn = (int) (( (float) rand() / RAND_MAX) * (width - 2) + 1);
    grid[0][randSpawn].setType(Cell::Type::START);
}
```

To create the maze, we generate `Wall` cells for every cell in this position `x % 2 == 0 && y % 2 == 0`.
Which may look something like this: 

>W     W     W     W
>
>W     W     W     W
>
>W     W     W     W
>
>W     W     W     W

To generate the maze portion, all we need to do is randomly connect the walls together, for example:

> W W W     W     W
>                           W
> W     W W W     W
> W
> W W W     W     W
> W              W 
> W     W     W W W

**Grid class** (Drawing the Maze)

```cpp
void draw() {
    std::string display = "";
    for (int y = 0; y < width; y++) {
        for (int x = 0; x < height; x++) {
            switch (grid[y][x].getType()) {
                case Cell::Type::WALL:
                    display += "# ";
                    break;
                case Cell::Type::SEARCH:
                    display += "O ";
                    break;
                case Cell::Type::START:
                    display += "S ";
                    break;
                case Cell::Type::GOAL:
                    display += "X ";
                    break;
                case Cell::Type::EMPTY:
                    display += "  ";
                    break;
            }
        }
        display += "\n";
    }
    std::cout << "Grid:\n" << display;
}
```

An appropriate ASCII letter is printed based on a `Cell`'s type.

**Searcher class**

```cpp
class Searcher {
public:
    static Cell* breadthFirstSearch (Grid *grid, Cell *start) {

        std::queue<Cell*> frontier;
        frontier.push(start);
        start->visited = true;

        while(!frontier.empty()) {
            
            // get and remove current cell from frontier
            Cell* cell = frontier.front();
            frontier.pop();

            // get neighbors of current cell
            std::vector<Cell*> neighbors = grid->neighborsOf(*cell);
            
            for (Cell* neighbor : neighbors) {
                if (!neighbor->visited) {  // check all the neighbors of the current cell for those that are not visited

                    frontier.push(neighbor);  // creating new frontier
                    neighbor->see();  // visited = true

                    grid->addPath(neighbor, cell);  // for figuring out the path afterwards

                    if (neighbor->getType() == Cell::Type::GOAL) {  // goal found
                        return neighbor;  // end cell
                    }
                }
            }
        }
        return nullptr;  // nullptr returned if end is not reachable
    }
};
```

The `Searcher` class is where we implement the BFS algorithm. I may want to implement other algorithms, and I can do that in this class. Each searcher method should return a pointer to a `GOAL` cell.



### Sample output

<img src="images\before.PNG" alt="before" style="zoom:80%;" />

<img src="images\solved.PNG" alt="solved" style="zoom:80%;" />