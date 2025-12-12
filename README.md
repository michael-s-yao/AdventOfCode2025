# [Advent of Code 2025](https://adventofcode.com/2025): C++ Solutions

[![LICENSE](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE.md)
[![CONTACT](https://img.shields.io/badge/contact-michael.yao%40pennmedicine.upenn.edu-blue)](mailto:michael.yao@pennmedicine.upenn.edu)

## Usage

To solve all the puzzles, first download your puzzle inputs into a new `data` subdirectory in this repository. The puzzle input for day `[DAY_NUM]` should be in `data/[DAY_NUM].txt`. You can then run

```
cd ~
git clone https://github.com/michael-s-yao/AdventOfCode2025
cd AdventOfCode2025
docker build -t cpp:latest . && docker run -it cpp:latest bash
```

If you'd only like to solve the puzzle only for day `[DAY_NUM]`, you can run

```
bash solve.sh [DAY_NUM]
```

Happy holidays! &#x1F384;

## License

This repository is MIT licensed (see [LICENSE](LICENSE)).