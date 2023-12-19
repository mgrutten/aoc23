#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <set>
#include <memory>
#include <algorithm>
#include <eigen3/Eigen/Eigen>

Eigen::MatrixXi load_puzzle(std::ifstream &infile)
{
    Eigen::MatrixXi input;

    std::string line;
    while (std::getline(infile, line) && !line.empty())
    {
        int curr_rows = input.rows();
        input.conservativeResize(curr_rows + 1, line.size());

        for (int c = 0; c < line.size(); ++c)
        {
            input(curr_rows, c) = line.at(c) - '0';
        }
    }

    return input;
}

enum Direction
{
    RIGHT,
    UP,
    LEFT,
    DOWN
};

const std::map<Direction, std::string> direction_str = {
    {RIGHT, "RIGHT"},
    {UP, "UP"},
    {LEFT, "LEFT"},
    {DOWN, "DOWN"}};

struct Index
{
    long row;
    long col;

    char val_at(const Eigen::MatrixXi &data) const
    {
        return data(row, col);
    }

    void move(const Direction d, const Eigen::MatrixXi &data)
    {
        switch (d)
        {
        case RIGHT:
            col = std::min(data.cols() - 1, col + 1);
            break;
        case UP:
            row = std::max(0L, row - 1);
            break;
        case LEFT:
            col = std::max(0L, col - 1);
            break;
        case DOWN:
            row = std::min(data.rows() - 1, row + 1);
            break;
        }
    }

    bool isedge(const Direction d, const Eigen::MatrixXi &data)
    {
        switch (d)
        {
        case RIGHT:
            return col == data.cols() - 1;
        case UP:
            return row == 0;
        case LEFT:
            return col == 0;
        case DOWN:
            return row == data.rows() - 1;
        }
        return false;
    }

    friend bool operator<(const Index &l, const Index &r)
    {
        return std::tie(l.row, l.col) < std::tie(r.row, r.col);
    }

    friend bool operator==(const Index &l, const Index &r)
    {
        return std::tie(l.row, l.col) == std::tie(r.row, r.col);
    }

    friend std::ostream &operator<<(std::ostream &os, const Index &index)
    {
        os << "(" << index.row << "," << index.col << ")";
        return os;
    }
};

struct Node
{
    Index curr;
    std::shared_ptr<Node> parent;
    long cost;
    Direction dirn;
    int steps;

    friend bool operator<(const Node &l, const Node &r)
    {
        return l.cost < r.cost;
    }

    friend bool operator>(const Node &l, const Node &r)
    {
        return l.cost > r.cost;
    }
};

struct VisitedNode
{
    Index curr;
    Direction dirn;
    int steps;

    friend bool operator<(const VisitedNode &l, const VisitedNode &r)
    {
        return std::tie(l.curr, l.dirn, l.steps) < std::tie(r.curr, r.dirn, r.steps);
    }
};

void make_move(const std::shared_ptr<Node> min_node,
               const Direction dirn,
               const int steps,
               const Eigen::MatrixXi &puzzle,
               std::priority_queue<Node, std::vector<Node>, std::greater<Node>> &queue)
{
    if (!min_node->curr.isedge(dirn, puzzle))
    {
        // std::cout << direction_str.at(dirn) << "\n";
        Index ind(min_node->curr);
        ind.move(dirn, puzzle);
        Node node({ind,
                   min_node,
                   min_node->cost + ind.val_at(puzzle),
                   dirn,
                   steps});
        queue.push(node);
    }
}

int main()
{
    std::ifstream infile("input.txt");
    Eigen::MatrixXi puzzle = load_puzzle(infile);

    Index target{puzzle.rows() - 1, puzzle.cols() - 1};

    Node start{{0, 0}, nullptr, 0, UP};
    std::shared_ptr<Node> start_ptr = std::make_shared<Node>(start);

    std::set<VisitedNode> visited;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> queue;

    queue.push({{0, 1}, start_ptr, puzzle(0, 1), RIGHT, 1});
    queue.push({{1, 0}, start_ptr, puzzle(1, 0), DOWN, 1});

    long path_length = 0;
    std::shared_ptr<Node> final_node;
    long count = 0;
    while (!queue.empty())
    {
        std::shared_ptr<Node> min_node = std::make_shared<Node>(queue.top());
        queue.pop();

        VisitedNode key{min_node->curr, min_node->dirn, min_node->steps};
        if (visited.count(key) == 1)
        {
            // std::cout << "Node exists"
            //           << "\n";
            // if (visited.at(key) < min_node->cost)
            //{
            continue;
            // std::cout << "Node same"
            //           << "\n";
            //}
        }
        visited.insert(key);

        // std::cout << "Min cost: " << min_node->cost << "\n";
        if (min_node->curr == target)
        {
            // std::cout << "Min cost: " << min_node->cost << "\n";
            final_node = min_node;
            path_length = min_node->cost; // + target.val_at(puzzle);
            break;
        }

        // Turn left
        switch (min_node->dirn)
        {
        case RIGHT:
            make_move(min_node, UP, 1, puzzle, queue);
            break;
        case LEFT:
            make_move(min_node, DOWN, 1, puzzle, queue);
            break;
        case UP:
            make_move(min_node, LEFT, 1, puzzle, queue);
            break;
        case DOWN:
            make_move(min_node, RIGHT, 1, puzzle, queue);
            break;
        }

        // Step in current direction
        if (min_node->steps < 3)
        {
            // std::cout << "Continue ";
            make_move(min_node, min_node->dirn, min_node->steps + 1, puzzle, queue);
        }

        // Turn right
        switch (min_node->dirn)
        {
        case RIGHT:
            make_move(min_node, DOWN, 1, puzzle, queue);
            break;
        case LEFT:
            make_move(min_node, UP, 1, puzzle, queue);
            break;
        case UP:
            make_move(min_node, RIGHT, 1, puzzle, queue);
            break;
        case DOWN:
            make_move(min_node, LEFT, 1, puzzle, queue);
            break;
        }

        // count += 1;
        // if (count > 1)
        //{
        //    break;
        // }
    }

    std::cout << "Shortest path: " << path_length << "\n";
    /*
    Eigen::MatrixXi path(puzzle.rows(), puzzle.cols());
    path.fill(0);
    std::shared_ptr<Node> curr(final_node);
    while (curr != nullptr)
    {
        // std::cout << curr->curr << "\n";
        path(curr->curr.row, curr->curr.col) = curr->dirn + 1;
        curr = curr->parent;
    }
    std::cout << puzzle << "\n\n";
    std::cout << path << "\n";
    */
}
