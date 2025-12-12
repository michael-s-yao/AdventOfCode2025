// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<algorithm>
#include<fstream>
#include<iostream>
#include<string>
#include<utility>
#include<vector>

void read_inp(
  const std::string fn,
  std::vector<std::pair<uint64_t, uint64_t>>& ranges,
  std::vector<uint64_t>& ingredients
);

int main() {
  const std::string fn = "data/05.txt";
  std::vector<std::pair<uint64_t, uint64_t>> ranges;
  std::vector<uint64_t> ingredients;
  read_inp(fn, ranges, ingredients);

  uint64_t is_fresh, total_a = 0;
  for (const uint64_t& x : ingredients) {
    is_fresh = 0;
    for (std::pair<uint64_t, uint64_t> lim : ranges)
      if ((is_fresh = (lim.first <= x && x <= lim.second)))
        break;
    total_a += is_fresh;
  }

  std::vector<std::pair<uint64_t, uint64_t>> to_remove;
  for (std::pair<uint64_t, uint64_t> lim : ranges)
    if (lim.first > lim.second)
      to_remove.push_back(lim);
  for (std::pair<uint64_t, uint64_t> lim : to_remove)
    ranges.erase(std::remove(ranges.begin(), ranges.end(), lim), ranges.end());

  std::sort(ranges.begin(), ranges.end());
  std::vector<std::pair<uint64_t, uint64_t>> merged_ranges;
  uint64_t low = ranges.front().first, high = ranges.front().second;
  for (size_t i = 1; i < ranges.size(); i++) {
    if (ranges[i].first > high + 1) {
      merged_ranges.push_back(std::make_pair(low, high));
      low = ranges[i].first, high = ranges[i].second;
    } else {
      high = std::max(high, ranges[i].second);
    }
  }
  merged_ranges.push_back(std::make_pair(low, high));

  uint64_t total_b = 0;
  for (std::pair<uint64_t, uint64_t> lim : merged_ranges)
    total_b += lim.second - lim.first + 1;

  std::cout << "Part A: " << total_a << std::endl;
  std::cout << "Part B: " << total_b << std::endl;

  return 0;
}

void read_inp(
  const std::string fn,
  std::vector<std::pair<uint64_t, uint64_t>>& ranges,
  std::vector<uint64_t>& ingredients
) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::string line;
  size_t idx;
  while (std::getline(file, line)) {
    if (!line.size())
      continue;
    if ((idx = line.find('-')) != std::string::npos)
      ranges.push_back(
        std::make_pair(
          std::stol(line.substr(0, idx)), std::stol(line.substr(idx + 1))));
    else
      ingredients.push_back(std::stol(line));
  }

  file.close();
}
