// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<algorithm>
#include<fstream>
#include<iostream>
#include<set>
#include<sstream>
#include<string>
#include<utility>
#include<vector>

struct Point {
  int x, y;
  bool operator<(const Point& other) const {
    return x < other.x || (x == other.x && y < other.y);
  }
};

struct Spec {
  int num_rows, num_cols;
  std::vector<size_t> counts;
};

class Shape {
 public:
  std::vector<Point> cells;

  explicit Shape(const std::vector<std::string>& pattern) {
    for (size_t row = 0; row < pattern.size(); row++)
      for (size_t col = 0; col < pattern[row].size(); col++)
        if (pattern[row][col] == '#')
          cells.push_back({static_cast<int>(row), static_cast<int>(col)});
    normalize();
  }

  Shape() {
    return;
  }

  void normalize() {
    int min_x = cells[0].x, min_y = cells[0].y;
    for (size_t i = 0; i < cells.size(); i++) {
      min_x = (min_x < cells[i].x) ? min_x : cells[i].x;
      min_y = (min_y < cells[i].y) ? min_y : cells[i].y;
    }
    for (size_t i = 0; i < cells.size(); i++) {
        cells[i].x -= min_x;
        cells[i].y -= min_y;
    }
  }

  Shape rotate() const {
    Shape result;
    for (size_t i = 0; i < cells.size(); i++)
      result.cells.push_back({-cells[i].y, cells[i].x});
    result.normalize();
    return result;
  }

  Shape flip() const {
    Shape result;
    for (size_t i = 0; i < cells.size(); i++)
      result.cells.push_back({-cells[i].x, cells[i].y});
    result.normalize();
    return result;
  }

  std::vector<Shape> orientations() const {
    std::set<std::vector<Point>> unique;
    std::vector<Shape> orientations;

    Shape cur = *this;
    std::vector<Point> key;
    for (int rot = 0; rot < 4; rot++) {
      key = cur.cells;
      std::sort(key.begin(), key.end());
      if (!unique.count(key)) {
        unique.insert(key);
        orientations.push_back(cur);
      }
      cur = cur.rotate();
    }

    cur = cur.flip();
    for (int rot = 0; rot < 4; rot++) {
      key = cur.cells;
      std::sort(key.begin(), key.end());
      if (!unique.count(key)) {
        unique.insert(key);
        orientations.push_back(cur);
      }
      cur = cur.rotate();
    }

    return orientations;
  }
};

class Board {
 public:
  int width, height;
  std::vector<std::vector<bool>> occupied;

  explicit Board(Spec spec) {
    width = spec.num_cols;
    height = spec.num_rows;
    for (int i = 0; i < spec.num_rows; i++)
      occupied.push_back(std::vector<bool>(spec.num_cols, false));
  }

  bool can_place(const Shape& shape, int start_x, int start_y) {
    int x, y;
    for (Point cell : shape.cells) {
      x = start_x + cell.x;
      y = start_y + cell.y;
      if (x < 0 || x >= width || y < 0 || y >= height || occupied[y][x])
        return false;
    }
    return true;
  }

  void place(const Shape& shape, int start_x, int start_y) {
    for (Point cell : shape.cells)
      occupied[start_y + cell.y][start_x + cell.x] = true;
  }

  void remove(const Shape& shape, int start_x, int start_y) {
    for (Point cell : shape.cells)
      occupied[start_y + cell.y][start_x + cell.x] = false;
  }
};

void read_inp(
  const std::string fn, std::vector<Shape>& templates, std::vector<Spec>& specs
);

bool solve(Spec spec, const std::vector<Shape>& shapes);

int main() {
  const std::string fn = "data/12.txt";
  std::vector<Shape> shapes;
  std::vector<Spec> specs;
  read_inp(fn, shapes, specs);

  int total_a = 0;
  for (Spec spec : specs)
    total_a += solve(spec, shapes);

  std::cout << "Part A: " << total_a << std::endl;

  return 0;
}

void read_inp(
  const std::string fn, std::vector<Shape>& templates, std::vector<Spec>& specs
) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::vector<std::vector<std::string>> patterns;
  std::string line, tmp;
  int num_rows, num_cols;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;
    if ((line.find(':') != std::string::npos) && (
      line.find('x') == std::string::npos))
      continue;
    if (line.find('x') == std::string::npos) {
      if (patterns.empty() || patterns.back().size() >= 3)
        patterns.push_back(std::vector<std::string>());
      patterns.back().push_back(line);
    } else {
      for (size_t i = 0; i < line.size(); i++)
        if (line[i] == ':' || line[i] == 'x')
          line[i] = ' ';
      std::stringstream ss(line);
      ss >> tmp;
      num_cols = std::stoi(tmp);
      ss >> tmp;
      num_rows = std::stoi(tmp);
      std::vector<size_t> counts;
      while (ss >> tmp)
        counts.push_back(std::stoull(tmp));
      if (counts.size() != patterns.size()) {
        std::cerr << "Error: Malformed data in " << fn << std::endl;
        exit(1);
      }

      specs.push_back({num_rows, num_cols, counts});
    }
  }

  templates.clear();
  for (std::vector<std::string> pattern : patterns)
    templates.push_back(Shape(pattern));

  file.close();
}

bool solve(Spec spec, const std::vector<Shape>& shapes) {
  // Backtracking is not feasible here. Simple heuristic ends up
  // solving the problem.
  Board board(spec);
  size_t area = static_cast<size_t>(board.height * board.width);
  size_t total_cells = 0;
  for (size_t i = 0; i < shapes.size(); i++)
    total_cells += spec.counts[i] * shapes[i].cells.size();
  if (total_cells >= area)
    return false;

  return shapes.size() <= (area / 9);
}
