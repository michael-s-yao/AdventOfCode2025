// Copyright 2025 Michael S. Yao (@michael-s-yao)
#include<cmath>
#include<deque>
#include<fstream>
#include<iostream>
#include<limits>
#include<sstream>
#include<string>
#include<unordered_set>
#include<utility>
#include<vector>

std::vector<std::string> read_inp(const std::string fn);

void parse(
  const std::string& x,
  std::vector<bool>& target,
  std::vector<std::vector<size_t>>& buttons,
  std::vector<int>& joltage_reqs
);

int part_a(
  const std::vector<bool>& target,
  const std::vector<std::vector<size_t>>& buttons
);

int part_b(
  const std::vector<std::vector<size_t>>& buttons,
  const std::vector<int>& joltage_reqs
);

double simplex_solve(
  const std::vector<std::vector<double>>& A,
  const std::vector<double>& b,
  const std::vector<double>& c
);

int main() {
  const std::string fn = "data/10.txt";
  const std::vector<std::string> configs = read_inp(fn);

  int total_a = 0, total_b = 0;
  std::vector<bool> target;
  std::vector<std::vector<size_t>> buttons;
  std::vector<int> joltage_reqs;
  for (size_t i = 0; i < configs.size(); i++) {
    parse(configs[i], target, buttons, joltage_reqs);
    total_a += part_a(target, buttons);
    total_b += part_b(buttons, joltage_reqs);
  }

  std::cout << "Part A: " << total_a << std::endl;
  std::cout << "Part B: " << total_b << std::endl;

  return 0;
}

std::vector<std::string> read_inp(const std::string fn) {
  std::ifstream file(fn);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open " << fn << std::endl;
    exit(1);
  }

  std::vector<std::string> data;
  std::string line;
  while (std::getline(file, line))
    data.push_back(line);

  file.close();
  return data;
}

void parse(
  const std::string& x,
  std::vector<bool>& target,
  std::vector<std::vector<size_t>>& buttons,
  std::vector<int>& joltage_reqs
) {
  target.clear();
  size_t low = x.find('['), high = x.find(']');
  if (low == std::string::npos || high == std::string::npos || high <= low) {
    std::cerr << "Error: Malformed data: " << x << std::endl;
    exit(1);
  }
  for (size_t i = low + 1; i < high; i++)
    target.push_back(x[i] == '#');

  std::string tok;

  buttons.clear();
  size_t pos_open = x.find('(', high), l_brace = x.find('{', high), pos_close;
  if (pos_open == std::string::npos || l_brace == std::string::npos) {
    std::cerr << "Error: Malformed data: " << x << std::endl;
    exit(1);
  }
  while (pos_open != std::string::npos && (pos_open < l_brace)) {
    if ((pos_close = x.find(')', pos_open)) == std::string::npos)
      break;
    buttons.push_back(std::vector<size_t>());
    std::stringstream ss(x.substr(pos_open + 1, pos_close - pos_open - 1));
    while (std::getline(ss, tok, ','))
      buttons.back().push_back(static_cast<size_t>(std::stoull(tok)));
    pos_open = x.find('(', pos_close);
  }

  joltage_reqs.clear();
  size_t r_brace = x.find('}', l_brace);
  if (r_brace == std::string::npos || r_brace <= l_brace) {
    std::cerr << "Error: Malformed data: " << x << std::endl;
    exit(1);
  }
  std::stringstream ss(x.substr(l_brace + 1, r_brace - l_brace - 1));
  while (std::getline(ss, tok, ','))
    joltage_reqs.push_back(std::stoi(tok));
}

int part_a(
  const std::vector<bool>& target,
  const std::vector<std::vector<size_t>>& buttons
) {
  std::deque<std::pair<std::vector<bool>, int>> queue;
  std::unordered_set<std::vector<bool>> visited;

  std::vector<bool> start(target.size(), false);
  queue.push_back({start, 0});
  visited.insert(start);
  std::pair<std::vector<bool>, int> curr;
  while (!queue.empty()) {
    curr = queue.front();
    if (curr.first == target)
      return curr.second;
    for (std::vector<size_t> button : buttons) {
      std::vector<bool> next = curr.first;
      for (const size_t& loc : button)
        next[loc] = !next[loc];
      if (visited.find(next) != visited.end())
        continue;
      visited.insert(next);
      queue.push_back({next, curr.second + 1});
    }
    queue.pop_front();
  }
  std::cerr << "Error: Failed to find solution." << std::endl;
  exit(1);
}

int part_b(
  const std::vector<std::vector<size_t>>& buttons,
  const std::vector<int>& joltage_reqs
) {
  std::vector<std::vector<double>> A(
    joltage_reqs.size(), std::vector<double>(buttons.size(), 0.0));

  for (size_t j = 0; j < buttons.size(); j++)
    for (const size_t& i : buttons[j])
      A[i][j] = 1.0;

  std::vector<double> b(joltage_reqs.size());
  for (size_t j = 0; j < joltage_reqs.size(); j++)
    b[j] = static_cast<double>(joltage_reqs[j]);

  std::vector<double> c(buttons.size(), 1.0);

  double result = simplex_solve(A, b, c);

  return static_cast<int>(std::ceil(result));
}

double simplex_solve(
  const std::vector<std::vector<double>>& A,
  const std::vector<double>& b,
  const std::vector<double>& c
) {
  const double M = 1000000.0, eps = 1e-12;

  /* Big-M method. */
  std::vector<std::vector<double>> tab(
    A.size() + 1, std::vector<double>(A.size() + c.size() + 1, 0.0));
  std::vector<size_t> basis(A.size());

  // 1. Fill constraints and artificial variables.
  for (size_t i = 0; i < A.size(); i++) {
    for (size_t j = 0; j < c.size(); j++)
        tab[i][j] = A[i][j];
    tab[i][c.size() + i] = 1.0;
    tab[i][A.size() + c.size()] = b[i];
    basis[i] = c.size() + i;
  }

  // 2. Fill Objective Row.
  double zj;
  for (size_t j = 0; j < A.size() + c.size(); j++) {
    zj = 0.0;
    for (size_t i = 0; i < A.size(); i++)
      zj += M * tab[i][j];
    tab[A.size()][j] = ((j < c.size()) ? c[j] : M) - zj;
  }

  // 3. Update current objective value.
  double z_val = 0.0;
  for (size_t i = 0; i < A.size(); i++)
    z_val += M * tab[i][A.size() + c.size()];
  tab[A.size()][A.size() + c.size()] = -z_val;

  // 4. Simplex Iterations.
  int pivot_row, pivot_col, min_val;
  double min_ratio, ratio;
  while (true) {
    pivot_col = -1, min_val = -eps;

    for (size_t j = 0; j < A.size() + c.size(); j++)
      if (tab[A.size()][j] < min_val)
        min_val = tab[A.size()][j], pivot_col = j;
    if (pivot_col == -1)
      break;

    pivot_row = -1;
    min_ratio = std::numeric_limits<double>::infinity();
    for (size_t i = 0; i < A.size(); i++) {
      if (tab[i][pivot_col] > eps) {
        ratio = tab[i][A.size() + c.size()] / tab[i][pivot_col];
        if (ratio < min_ratio)
          min_ratio = ratio, pivot_row = i;
      }
    }
    if (pivot_row == -1)
      return -1.0;

    double pivot_ell = tab[pivot_row][pivot_col], factor;
    for (size_t j = 0; j <= A.size() + c.size(); j++)
      tab[pivot_row][j] /= pivot_ell;

    for (size_t i = 0; i <= A.size(); i++) {
      if (static_cast<int>(i) == pivot_row)
        continue;
      factor = tab[i][pivot_col];
      for (size_t j = 0; j <= A.size() + c.size(); j++)
        tab[i][j] -= factor * tab[pivot_row][j];
    }

    basis[pivot_row] = pivot_col;
  }

  // 5. Extract solution.
  for (size_t i = 0; i < A.size(); i++)
    if (basis[i] >= c.size() && std::abs(tab[i][A.size() + c.size()]) > eps)
      return -1.0;

  double total = 0.0;
  for (size_t i = 0; i < A.size(); i++)
    if (basis[i] < c.size())
      total += tab[i][A.size() + c.size()];
  return total;
}
