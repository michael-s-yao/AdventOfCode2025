// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<algorithm>
#include<fstream>
#include<iostream>
#include<string>
#include<utility>
#include<vector>

std::pair<std::vector<char>, std::vector<int>> read_inp(const std::string fn);

int main() {
  const std::string fn = "data/01.txt";
  std::pair<std::vector<char>, std::vector<int>> data = read_inp(fn);
  const std::vector<char> directions = data.first;
  const std::vector<int> rotations = data.second;

  int pos = 50, total_a = 0, total_b = 0, tmp, ell, r;
  const int d = 100;
  for (size_t i = 0; i < directions.size(); i++) {
    tmp = pos + ((directions[i] == 'L' ? -1 : 1) * rotations[i]);
    ell = std::min(pos, tmp), r = std::max(pos, tmp);
    total_b += (pos != 0) + (r - (r % d) - ell - (((-ell % d) + d) % d)) / d;
    pos = (tmp % d + d) % d;
    total_a += (pos == 0);
  }

  std::cout << "Part A: " << total_a << std::endl;
  std::cout << "Part B: " << total_b << std::endl;

  return 0;
}

std::pair<std::vector<char>, std::vector<int>> read_inp(const std::string fn) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::vector<char> directions;
  std::vector<int> rotations;

  std::string line;
  while (std::getline(file, line)) {
    directions.push_back(line[0]);
    rotations.push_back(std::stoi(line.substr(1)));
  }

  file.close();

  if (directions.size() != rotations.size()) {
    std::cerr << "Error: Malformed data in " << fn << std::endl;
    exit(1);
  }

  return std::make_pair(directions, rotations);
}
