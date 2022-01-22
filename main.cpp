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

    Cell () {
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
    Cell* getStartCell() {
        for (int x = 0; x < width; x++) {
            if (grid[0][x].getType() == Cell::Type::START) {
                return &grid[0][x];
            }
        }
        return &outOfBoundsCell;
    }

    std::vector<Cell*> neighborsOf(Cell cell) {  // get a cell's neighbors
        std::vector<Cell*> neighbors;
        int x = cell.x;
        int y = cell.y;

        Cell* neighbor = getCell(x + 1, y);
        if (inGridBounds(x + 1, y) && neighbor->getType() != Cell::Type::WALL) {
            neighbors.push_back(neighbor);
        }
        neighbor = getCell(x - 1, y);
        if (inGridBounds(x - 1, y) && neighbor->getType() != Cell::Type::WALL) {
            neighbors.push_back(neighbor);
        }
        neighbor = getCell(x, y + 1);
        if (inGridBounds(x, y + 1) && neighbor->getType() != Cell::Type::WALL) {
            neighbors.push_back(neighbor);
        }
        neighbor = getCell(x, y - 1);
        if (inGridBounds(x, y - 1) && neighbor->getType() != Cell::Type::WALL) {
            neighbors.push_back(neighbor);
        }

        return neighbors;
    }

    void setCell(int x, int y, Cell::Type type) {
        if (inGridBounds(x, y)) {
            grid[x][y].setType(type);
        }
    }

    void addPath(Cell* parent, Cell* child) {
        paths[parent] = child;
    }
    Cell* getPath(Cell* parent) {
        return paths[parent];
    }

    bool inGridBounds(int x, int y) {
        return (x < width && x >= 0) && (y < height && y >= 0);
    }

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
        grid[height - 1][width - 2].setType(Cell::Type::GOAL);
    }

    void clear() {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                grid[x][y].setType(Cell::Type::EMPTY);
                grid[x][y].visited = false;
            }
        }
        paths.clear();
    }

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
};

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
        return nullptr;
    }
};

int main() {
    int WIDTH = 51;
    int HEIGHT = 51;

    Grid grid = Grid(WIDTH, HEIGHT);
    grid.create();

    Cell* origin = Searcher::breadthFirstSearch(&grid, grid.getStartCell());  // the end cell

    while (origin == nullptr) {  // generate mazes until we find a valid one
        std::cout << "Generated grid - no path\n";
        grid.clear();
        grid.create();
        origin = Searcher::breadthFirstSearch(&grid, grid.getStartCell());  // map to every cell that could be accessed
    }

    grid.draw();  // draw initial empty maze

    std::cout << "Goal found \n";
    std::vector<Cell*> path;
    while (origin->getType() != Cell::Type::START) {
        path.push_back(origin);
        origin = grid.getPath(origin);
    }

    for (Cell* cell : path) {
        cell->setType(Cell::Type::SEARCH);
    }

    grid.draw();  // draw solved maze

    std::cin.get();
}