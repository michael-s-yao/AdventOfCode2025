// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<algorithm>
#include<fstream>
#include<iostream>
#include<limits>
#include<map>
#include<sstream>
#include<string>
#include<set>
#include<vector>

std::vector<std::vector<int>> read_inp(const std::string fn);

uint64_t num_paths(
  const size_t& i,
  const size_t& j,
  const std::vector<std::vector<int>>& A
);

int main() {
  const std::string fn = "data/11.txt";
  // Assume that the graph is acyclic (true for this problem).
  std::vector<std::vector<int>> A = read_inp(fn);
  const size_t n = A.size() - 1;

  std::cout << "Part A: " << num_paths(0, n, A) << std::endl;

  uint64_t b = num_paths(1, 2, A) * num_paths(2, 3, A) * num_paths(3, n, A);
  b += num_paths(1, 3, A) * num_paths(3, 2, A) * num_paths(2, n, A);
  std::cout << "Part B: " << b << std::endl;

  return 0;
}

std::vector<std::vector<int>> read_inp(const std::string fn) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::map<std::string, std::vector<std::string>> data;
  std::string line;
  while (std::getline(file, line)) {
    for (size_t i = 0; i < line.size(); i++)
      if (line[i] == ':')
        line[i] = ' ';

    std::stringstream ss(line);
    std::string src;
    ss >> src;
    if (data.count(src)) {
      std::cerr << "Error: Malformed data in " << fn << std::endl;
      exit(1);
    }

    std::vector<std::string> dsts;
    std::string dst;
    while (ss >> dst)
      dsts.push_back(dst);
    data[src] = dsts;
  }

  file.close();

  std::set<std::string> unique_nodes;
  for (std::pair<std::string, std::vector<std::string>> kv : data) {
    unique_nodes.insert(kv.first);
    for (std::string dst : kv.second)
      unique_nodes.insert(dst);
  }
  std::vector<std::string> nodes(unique_nodes.begin(), unique_nodes.end());;
  nodes.erase(std::remove(nodes.begin(), nodes.end(), "you"), nodes.end());
  nodes.erase(std::remove(nodes.begin(), nodes.end(), "svr"), nodes.end());
  nodes.erase(std::remove(nodes.begin(), nodes.end(), "fft"), nodes.end());
  nodes.erase(std::remove(nodes.begin(), nodes.end(), "dac"), nodes.end());
  nodes.erase(std::remove(nodes.begin(), nodes.end(), "out"), nodes.end());
  nodes.push_back("out");
  nodes.insert(nodes.begin(), "dac");
  nodes.insert(nodes.begin(), "fft");
  nodes.insert(nodes.begin(), "svr");
  nodes.insert(nodes.begin(), "you");

  std::vector<std::vector<int>> A(
    nodes.size(), std::vector<int>(nodes.size(), 0));
  size_t row, col;
  for (std::pair<std::string, std::vector<std::string>> kv : data) {
    row = std::distance(
      nodes.begin(), std::find(nodes.begin(), nodes.end(), kv.first));
    for (std::string dst : kv.second) {
      col = std::distance(
        nodes.begin(), std::find(nodes.begin(), nodes.end(), dst));
      A[row][col] = 1;
    }
  }
  return A;
}

uint64_t find_paths(
  size_t cur,
  size_t tgt,
  const std::vector<std::vector<int>>& A,
  std::vector<uint64_t>& memo
) {
  if (cur == tgt)
    return 1;
  if (memo[cur] != std::numeric_limits<uint64_t>::max())
    return memo[cur];

  uint64_t count = 0;
  for (size_t next = 0; next < A.size(); next++)
    if (A[cur][next])
      count += find_paths(next, tgt, A, memo);
  return memo[cur] = count;
}

uint64_t num_paths(
  const size_t& i,
  const size_t& j,
  const std::vector<std::vector<int>>& A
) {
  if (i >= A.size() || j >= A.size())
    return 0;
  std::vector<uint64_t> memo(A.size(), std::numeric_limits<uint64_t>::max());
  return find_paths(i, j, A, memo);
}
