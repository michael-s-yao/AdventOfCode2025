// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<algorithm>
#include<fstream>
#include<iostream>
#include<string>
#include<vector>

std::vector<std::vector<std::string>> read_inp(const std::string fn);

int main() {
  const std::string fn = "data/06.txt";
  const std::vector<std::vector<std::string>> data = read_inp(fn);

  uint64_t total_a = 0, total_b = 0, row_tmp, tmp, pow;
  char tmp_c;
  for (size_t i = 0; i < data.front().size(); i++) {
    /* Part A. */
    row_tmp = data.back()[i].front() == '*';
    for (size_t r = 0; r < data.size() - 1; r++) {
      tmp = std::stol(data[r][i]);
      while (tmp % 10 == 0)
        tmp = tmp / 10;
      if (data.back()[i].front() == '+')
        row_tmp += tmp;
      else
        row_tmp *= tmp;
    }
    total_a += row_tmp;

    /* Part B. */
    row_tmp = data.back()[i].front() == '*';
    for (size_t c = 0; c < data.front()[i].size(); c++) {
      pow = 1, tmp = 0;
      for (size_t r = 0; r < data.size(); r++) {
        tmp_c = data[data.size() - 1 - r][i][c];
        if ('0' > tmp_c || '9' < tmp_c)
          continue;
        tmp += (tmp_c - '0') * pow;
        pow *= 10;
      }
      if (tmp == 0)
        continue;
      if (data.back()[i].front() == '+')
        row_tmp += tmp;
      else
        row_tmp *= tmp;
    }
    total_b += row_tmp;
  }

  std::cout << "Part A: " << total_a << std::endl;
  std::cout << "Part B: " << total_b << std::endl;

  return 0;
}

std::vector<std::vector<std::string>> read_inp(const std::string fn) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line))
    lines.push_back(line);

  std::vector<std::vector<std::string>> data;
  size_t pos, found;
  while (true) {
    found = 0, pos = 0;
    while (!found) {
      for (std::string line : lines) {
        if (!(found = (line[pos] == ' ')))
          break;
      }
      pos++;
    }
    if (!(pos = std::min(pos, lines.front().size())))
      break;
    std::vector<std::string> row;
    for (size_t i = 0; i < lines.size(); i++) {
      row.push_back(lines[i].substr(0, pos));
      lines[i] = lines[i].substr(pos);
    }
    data.push_back(row);
  }

  std::vector<std::vector<std::string>> transposed;
  for (size_t i = 0; i < data.front().size(); i++) {
    transposed.push_back(std::vector<std::string>());
    for (size_t j = 0; j < data.size(); j++)
      transposed.back().push_back(data[j][i]);
  }

  file.close();
  return transposed;
}
