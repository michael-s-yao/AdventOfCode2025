// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<algorithm>
#include<fstream>
#include<functional>
#include<iostream>
#include<map>
#include<numeric>
#include<sstream>
#include<string>
#include<utility>
#include<vector>

std::vector<std::tuple<uint64_t, uint64_t, uint64_t>> read_inp(
  const std::string fn
);

std::vector<std::vector<uint64_t>> dmatrix(
  const std::vector<std::tuple<uint64_t, uint64_t, uint64_t>>& points
);

int main() {
  const std::string fn = "data/08.txt";
  const std::vector<std::tuple<uint64_t, uint64_t, uint64_t>> grid = read_inp(
    fn);
  const std::vector<std::vector<uint64_t>> distances = dmatrix(grid);
  const size_t max_connections = 1000;

  std::map<uint64_t, std::vector<std::pair<size_t, size_t>>> dist2pairs;
  for (size_t i = 0; i + 1 < distances.size(); i++) {
    for (size_t j = i + 1; j < distances.size(); j++) {
      if (!dist2pairs.count(distances[i][j]))
        dist2pairs[distances[i][j]] = std::vector<std::pair<size_t, size_t>>();
      dist2pairs[distances[i][j]].push_back(std::make_pair(i, j));
    }
  }

  std::vector<size_t> groups(grid.size());
  size_t p1, p2, g;
  bool is_done;

  /* Part A. */
  std::iota(groups.begin(), groups.end(), 0);
  is_done = false;
  size_t num_connections = 0;
  for (
    std::pair<uint64_t, std::vector<std::pair<size_t, size_t>>> kv : dist2pairs
  ) {
    for (std::pair<size_t, size_t> pair : kv.second) {
      p1 = pair.first, p2 = pair.second;
      if ((is_done = ++num_connections >= max_connections))
        break;
      if (groups[p1] == (g = groups[p2]))
        continue;
      for (size_t i = 0; i < groups.size(); i++)
        if (groups[i] == g)
          groups[i] = groups[p1];
    }
  }

  std::vector<size_t> gsizes(grid.size(), 0);
  for (const size_t& g : groups)
    gsizes[g]++;
  std::sort(gsizes.begin(), gsizes.end(), std::greater<size_t>());

  /* Part B. */
  std::iota(groups.begin(), groups.end(), 0);
  is_done = false;
  for (
    std::pair<uint64_t, std::vector<std::pair<size_t, size_t>>> kv : dist2pairs
  ) {
    is_done = true;
    for (size_t i = 1; i < groups.size(); i++)
      if (!(is_done = (groups[i] == groups[0])))
        break;
    if (is_done)
      break;

    for (std::pair<size_t, size_t> pair : kv.second) {
      p1 = pair.first, p2 = pair.second;
      if (groups[p1] == (g = groups[p2]))
        continue;
      for (size_t i = 0; i < groups.size(); i++)
        if (groups[i] == g)
          groups[i] = groups[p1];
    }
  }

  size_t total_a = gsizes[0] * gsizes[1] * gsizes[2];
  uint64_t total_b = std::get<0>(grid[p1]) * std::get<0>(grid[p2]);
  std::cout << "Part A: " << total_a << std::endl;
  std::cout << "Part B: " << total_b << std::endl;

  return 0;
}

std::vector<std::tuple<uint64_t, uint64_t, uint64_t>> read_inp(
  const std::string fn
) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::vector<std::tuple<uint64_t, uint64_t, uint64_t>> data;
  std::string line;
  std::string x, y, z;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    if (
      !std::getline(ss, x, ',') || !std::getline(ss, y, ',') || !std::getline(
        ss, z, ',')) {
      std::cerr << "Error: Malformed data in " << fn << std::endl;
      exit(1);
    }
    data.push_back(std::make_tuple(std::stoi(x), std::stoi(y), std::stoi(z)));
  }

  file.close();
  return data;
}

std::vector<std::vector<uint64_t>> dmatrix(
  const std::vector<std::tuple<uint64_t, uint64_t, uint64_t>>& points
) {
  std::vector<std::vector<uint64_t>> data(
    points.size(), std::vector<uint64_t>(points.size(), 0));
  uint64_t dx, dy, dz, ds;
  for (size_t i = 0; i < points.size() - 1; i++) {
    for (size_t j = i + 1; j < points.size(); j++) {
      dx = std::get<0>(points[i]) - std::get<0>(points[j]);
      if (std::get<0>(points[i]) < std::get<0>(points[j]))
        dx = std::get<0>(points[j]) - std::get<0>(points[i]);

      dy = std::get<1>(points[i]) - std::get<1>(points[j]);
      if (std::get<1>(points[i]) < std::get<1>(points[j]))
        dy = std::get<1>(points[j]) - std::get<1>(points[i]);

      dz = std::get<2>(points[i]) - std::get<2>(points[j]);
      if (std::get<2>(points[i]) < std::get<2>(points[j]))
        dz = std::get<2>(points[j]) - std::get<2>(points[i]);

      ds = (dx * dx) + (dy * dy) + (dz * dz);
      data[i][j] = ds, data[j][i] = ds;
    }
  }
  return data;
}
