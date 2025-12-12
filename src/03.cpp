// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<cmath>
#include<fstream>
#include<iostream>
#include<string>
#include<vector>

std::vector<std::vector<int>> read_inp(const std::string fn);

uint64_t find_max_joltage(std::vector<int> batteries, const int num_digits);

int main() {
  const std::string fn = "data/03.txt";
  std::vector<std::vector<int>> data = read_inp(fn);

  uint64_t total_a = 0, total_b = 0;
  for (std::vector<int> row : data)
    total_a += find_max_joltage(row, 2), total_b += find_max_joltage(row, 12);

  std::cout << "Part A: " << total_a << std::endl;
  std::cout << "Part B: " << total_b << std::endl;

  return 0;
}

std::vector<std::vector<int>> read_inp(const std::string fn) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::string line;
  std::vector<std::vector<int>> data;
  while (std::getline(file, line)) {
    std::vector<int> row;
    for (const char& digit : line)
      row.push_back(digit - '0');
    data.push_back(row);
  }

  file.close();
  return data;
}

uint64_t find_max_joltage(std::vector<int> batteries, const int num_digits) {
  size_t k = batteries.size() - num_digits;
  std::string s;
  for (const int& x : batteries) {
    while (s.size() && k > 0 && (s.back() - '0') < x)
      s.pop_back(), k -= 1;
    s.push_back(x + '0');
  }
  return std::stol(s.substr(0, s.size() - k));
}
