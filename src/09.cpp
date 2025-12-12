// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<algorithm>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<utility>
#include<vector>

std::vector<std::pair<uint64_t, uint64_t>> read_inp(const std::string fn);

bool is_point_tiled(
  std::pair<double, double>& loc,
  const std::vector<std::pair<uint64_t, uint64_t>>& red_tiles
);

bool is_segment_no_intersection(
  std::pair<double, double>& a,
  std::pair<double, double>& b,
  const std::vector<std::pair<uint64_t, uint64_t>>& red_tiles
);

int main() {
  const std::string fn = "data/09.txt";
  const std::vector<std::pair<uint64_t, uint64_t>> red_tiles = read_inp(fn);

  /* Part A. */
  uint64_t x, y, area, best_area_a = 0;
  for (size_t i = 0; i < red_tiles.size() - 1; i++) {
    for (size_t j = i + 1; j < red_tiles.size(); j++) {
      if (red_tiles[i].first > red_tiles[j].first)
        x = red_tiles[i].first - red_tiles[j].first;
      else
        x = red_tiles[j].first - red_tiles[i].first;

      if (red_tiles[i].second > red_tiles[j].second)
        y = red_tiles[i].second - red_tiles[j].second;
      else
        y = red_tiles[j].second - red_tiles[i].second;

      if ((area = (x + 1) * (y + 1)) > best_area_a)
        best_area_a = area;
    }
  }

  /* Part B. */
  bool is_valid;
  uint64_t best_area_b = 0;
  int xsgn, ysgn;
  double ax, ay, bx, by;
  std::pair<uint64_t, uint64_t> a, b;
  for (size_t i = 0; i < red_tiles.size(); i++) {
    a = red_tiles[i];
    for (size_t j = i + 1; j < red_tiles.size(); j++) {
      b = red_tiles[j];

      x = a.first - b.first, xsgn = a.first != b.first;
      if (a.first < b.first)
        x = b.first - a.first, xsgn = -1;
      y = a.second - b.second, ysgn = a.second != b.second;
      if (a.second < b.second)
        y = b.second - a.second, ysgn = -1;
      if ((area = (x + 1) * (y + 1)) <= best_area_b)
        continue;

      ax = a.first - (0.5 * xsgn), ay = a.second - (0.5 * ysgn);
      bx = b.first + (0.5 * xsgn), by = b.second + (0.5 * ysgn);

      std::vector<std::pair<double, double>> points = {
        {ax, ay}, {ax, by}, {bx, by}, {bx, ay}
      };

      is_valid = true;
      for (size_t k = 0; k < points.size(); k++) {
        size_t pk = (k == 0) ? points.size() - 1 : k - 1;

        if (!(is_valid = is_point_tiled(points[k], red_tiles)))
          break;
        else if (
          !(
            is_valid = is_segment_no_intersection(
              points[k], points[pk], red_tiles)
          )
        )
          break;
      }

      if (is_valid)
        best_area_b = area;
    }
  }

  std::cout << "Part A: " << best_area_a << std::endl;
  std::cout << "Part B: " << best_area_b << std::endl;

  return 0;
}

std::vector<std::pair<uint64_t, uint64_t>> read_inp(const std::string fn) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::vector<std::pair<uint64_t, uint64_t>> data;
  std::string line;
  std::string x, y;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    if (!std::getline(ss, x, ',') || !std::getline(ss, y, ',')) {
      std::cerr << "Error: Malformed data in " << fn << std::endl;
      exit(1);
    }
    data.push_back(std::make_pair(std::stol(x), std::stol(y)));
  }

  file.close();
  return data;
}

bool is_point_tiled(
  std::pair<double, double>& loc,
  const std::vector<std::pair<uint64_t, uint64_t>>& red_tiles
) {
  size_t num_intersections = 0, pi;
  for (size_t i = 0; i < red_tiles.size(); i++) {
    pi = (i == 0) ? red_tiles.size() - 1 : i - 1;
    if (red_tiles[i].first != red_tiles[pi].first)
      continue;
    if (static_cast<double>(red_tiles[i].first) < loc.first)
      continue;
    num_intersections += (
      ((static_cast<double>(red_tiles[i].second) < loc.second) && (
        loc.second < static_cast<double>(red_tiles[pi].second))) || (
        (static_cast<double>(red_tiles[i].second) > loc.second) && (
          loc.second > static_cast<double>(red_tiles[pi].second))));
  }
  return (num_intersections % 2);
}

bool is_segment_no_intersection(
  std::pair<double, double>& a,
  std::pair<double, double>& b,
  const std::vector<std::pair<uint64_t, uint64_t>>& red_tiles
) {
  size_t pi;
  if (a.first == b.first) {
    for (size_t i = 0; i < red_tiles.size(); i++) {
      pi = (i == 0) ? red_tiles.size() - 1 : i - 1;

      if (red_tiles[i].second != red_tiles[pi].second)
        continue;
      else if (std::min(a.second, b.second) >= red_tiles[i].second)
        continue;
      else if (red_tiles[i].second >= std::max(a.second, b.second))
        continue;
      else if (std::min(red_tiles[i].first, red_tiles[pi].first) >= a.first)
        continue;
      else if (a.first < std::max(red_tiles[i].first, red_tiles[pi].first))
        return false;
    }
  } else if (a.second == b.second) {
    for (size_t i = 0; i < red_tiles.size(); i++) {
      pi = (i == 0) ? red_tiles.size() - 1 : i - 1;

      if (red_tiles[i].first != red_tiles[pi].first)
        continue;
      else if (std::min(a.first, b.first) >= red_tiles[i].first)
        continue;
      else if (red_tiles[i].first >= std::max(a.first, b.first))
        continue;
      else if (std::min(red_tiles[i].second, red_tiles[pi].second) >= a.second)
        continue;
      else if (a.second < std::max(red_tiles[i].second, red_tiles[pi].second))
        return false;
    }
  }
  return true;
}
