// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<fstream>
#include<iostream>
#include<string>
#include<unordered_set>
#include<vector>

std::vector<std::vector<char>> read_inp(const std::string fn);

int main() {
  const std::string fn = "data/07.txt";
  const std::vector<std::vector<char>> grid = read_inp(fn);
  const size_t H = grid.size(), W = grid.front().size();

  int start_row, start_col;
  std::unordered_set<size_t> active_cols, next_active_cols;
  std::vector<uint64_t> counts(W, 0), next_counts(W, 0);
  for (size_t row = 0; row < H; row++) {
    for (size_t col = 0; col < W; col++) {
      if (grid[row][col] != 'S')
        continue;
      active_cols.insert(col);
      start_row = row, start_col = col;
      break;
    }
    if (start_row != -1)
      break;
  }
  counts[start_col] = 1;

  uint64_t total_a = 0, total_b = 0;
  for (size_t row = start_row + 1; row < H; row++) {
    next_active_cols.clear();
    std::fill(next_counts.begin(), next_counts.end(), 0);
    for (const size_t& col : active_cols) {
      if (grid[row][col] == '^') {
        total_a++;
        if (col - 1 >= 0) {
          next_active_cols.insert(col - 1);
          next_counts[col - 1] += counts[col];
        }
        if (col + 1 < W) {
          next_active_cols.insert(col + 1);
          next_counts[col + 1] += counts[col];
        }
      } else {
        next_active_cols.insert(col);
        next_counts[col] += counts[col];
      }
    }
    active_cols = next_active_cols;
    counts = next_counts;
  }

  for (const uint64_t& num : counts)
    total_b += num;

  std::cout << "Part A: " << total_a << std::endl;
  std::cout << "Part B: " << total_b << std::endl;

  return 0;
}

std::vector<std::vector<char>> read_inp(const std::string fn) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::vector<std::vector<char>> data;
  std::string line;
  while (std::getline(file, line)) {
    data.push_back(std::vector<char>());
    for (const char& c : line)
      data.back().push_back(c);
  }

  size_t len = data.front().size();
  for (std::vector<char> row : data) {
    if (row.size() != len) {
      std::cerr << "Error: Malformed data in " << fn << std::endl;
      exit(1);
    }
  }

  file.close();
  return data;
}
