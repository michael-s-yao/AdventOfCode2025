// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<fstream>
#include<iostream>
#include<string>
#include<unordered_set>

int64_t key(int32_t row, int32_t col) {
  return (static_cast<int64_t>(row) << 32) | col;
}

std::unordered_set<int64_t> read_inp(const std::string fn);

bool is_accessible(int64_t loc, const std::unordered_set<int64_t>& locations);

int main() {
  const std::string fn = "data/04.txt";
  std::unordered_set<int64_t> locations = read_inp(fn);
  std::unordered_set<int64_t> to_erase;

  int total_a = 0, total_b = 0, is_first_pass = 1;
  while (is_first_pass || to_erase.size()) {
    to_erase.clear();
    for (int64_t loc : locations) {
      if (!is_accessible(loc, locations))
        continue;
      to_erase.insert(loc);
      total_a += is_first_pass;
      total_b++;
    }
    for (int64_t loc : to_erase)
      locations.erase(loc);
    is_first_pass = 0;
  }

  std::cout << "Part A: " << total_a << std::endl;
  std::cout << "Part B: " << total_b << std::endl;

  return 0;
}

std::unordered_set<int64_t> read_inp(const std::string fn) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::unordered_set<int64_t> data;
  std::string line;
  size_t row = 0;
  while (std::getline(file, line)) {
    for (size_t col = 0; col < line.size(); col++)
      if (line[col] == '@')
        data.insert(key(row, col));
    row++;
  }

  file.close();
  return data;
}

bool is_accessible(int64_t loc, const std::unordered_set<int64_t>& locations) {
  int num_adj = -1;
  int32_t row = static_cast<int32_t>(loc >> 32);
  int32_t col = static_cast<int32_t>(loc);
  for (int32_t dr = -1; dr <= 1; dr++)
    for (int32_t dc = -1; dc <= 1; dc++)
      num_adj += locations.count(key(row + dr, col + dc));
  return num_adj < 4;
}
