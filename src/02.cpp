// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<utility>
#include<vector>

std::vector<std::pair<uint64_t, uint64_t>> read_inp(const std::string fn);

bool is_invalid_a(uint64_t n);

bool is_invalid_b(uint64_t n);

int main() {
  const std::string fn = "data/02.txt";
  const std::vector<std::pair<uint64_t, uint64_t>> data = read_inp(fn);

  uint64_t total_a = 0, total_b = 0;
  for (std::pair<uint64_t, uint64_t> range : data)
    for (uint64_t n = range.first; n <= range.second; n++)
      total_a += is_invalid_a(n) * n, total_b += is_invalid_b(n) * n;

  std::cout << "Part A: " << total_a << std::endl;
  std::cout << "Part B: " << total_b << std::endl;

  return 0;
}

std::vector<std::pair<uint64_t, uint64_t>> read_inp(const std::string fn) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::string line;
  if (!std::getline(file, line)) {
    std::cerr << "Error: Malformed data in " << fn << std::endl;
    exit(1);
  }

  std::vector<std::pair<uint64_t, uint64_t>> data;
  std::stringstream ss(line);
  std::string pair, st, ed;
  while (std::getline(ss, pair, ',')) {
    std::stringstream ss_pair(pair);
    if (!std::getline(ss_pair, st, '-') || !std::getline(ss_pair, ed, '-')) {
      std::cerr << "Error: Malformed data in " << fn << std::endl;
      exit(1);
    }
    data.push_back(std::make_pair(std::stol(st), std::stol(ed)));
  }

  file.close();
  return data;
}

bool is_invalid_a(uint64_t n) {
  std::string s = std::to_string(n);
  if (s.size() % 2)
    return false;
  return s.substr(0, s.size() / 2) == s.substr(s.size() / 2);
}

bool is_invalid_b(uint64_t n) {
  std::string s = std::to_string(n);
  std::string ds = s.substr(1) + s.substr(0, s.size() - 1);
  return (ds.find(s) != std::string::npos);
}
