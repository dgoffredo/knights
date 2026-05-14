#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <vector>

struct Point {
  int x;
  int y;
};

Point spiral_step(long n) {
  if (n == 0)
    return {0, 0};

  const long double nd = static_cast<long double>(n);

  const long k =
      static_cast<long>(std::ceill((std::sqrtl(nd + 1.0L) - 1.0L) / 2.0L));

  const long t = 2 * k + 1;
  const long m = t * t;
  const long d = (m - 1) - n;

  if (d < 2 * k) {
    return {int(k - d), int(-k)};
  }

  if (d < 4 * k) {
    return {int(-k), int(-k + (d - 2 * k))};
  }

  if (d < 6 * k) {
    return {int(-k + (d - 4 * k)), int(k)};
  }

  return {int(k), int(k - (d - 6 * k))};
}

long spiral_index(Point point) {
  const auto [x, y] = point;
  const long r =
      std::max(static_cast<long>(std::abs(x)), static_cast<long>(std::abs(y)));

  if (r == 0)
    return 0;

  const long side = 2 * r + 1;
  const long M = side * side - 1;

  // Bottom edge (includes bottom-right corner)
  if (y == -r) {
    return M - (r - x);
  }

  // Left edge
  if (x == -r) {
    return M - 2 * r - (y + r);
  }

  // Top edge
  if (y == r) {
    return M - 4 * r - (x + r);
  }

  // Right edge
  return M - 6 * r - (r - y);
}

void usage(const char *argv0, std::ostream &out) {
  out << "usage: " << argv0
      << " [NUM_COLORS]\n"
         "\n"
         "NUM_COLORS is the number of different colors of knights to place on "
         "the board.\n"
         "NUM_COLORS defaults to 1 if not specified.\n";
}

int main(int argc, char *argv[]) {
  int colors;
  switch (argc) {
  case 0: // fall through
  case 1:
    colors = 1;
    break;
  case 2:
    if (!std::strcmp(argv[1], "--help") || !std::strcmp(argv[1], "-h")) {
      usage(argv[0], std::cout);
      return 0;
    }
    colors = std::atoi(argv[1]);
    if (!colors) {
      usage(argv[0], std::cerr);
      return 1;
    }
    break;
  default:
    usage(argv[0], std::cerr);
    return 2;
  }

  std::vector<int> knights;

  const auto ruled_out = [&knights](int color, long step) {
    if (step < long(knights.size()) && knights[step] != 0) {
        return true;
    }

    const auto [x, y] = spiral_step(step);
    for (int dy : {+1, -1}) {
      for (int dx : {+1, -1}) {
        for (Point candidate : {Point{.x = x + dx, .y = y + 2 * dy},
                                Point{.x = x + 2 * dx, .y = y + dy}}) {
          const long candidate_step = spiral_index(candidate);
          if (candidate_step < long(knights.size()) &&
              knights[candidate_step] != 0 &&
              knights[candidate_step] != color) {
            return true;
          }
        }
      }
    }
    return false;
  };

  std::vector<long> consider_next(colors); // color-1 -> next step
  for (;;) {
    for (int color = 1; color < colors + 1; ++color) {
      long &step = consider_next[color - 1];
      while (ruled_out(color, step)) {
        ++step;
      }
      if (step + 1 > long(knights.size())) {
        knights.resize(step + 1);
      }
      knights[step] = color;
      const auto [x, y] = spiral_step(step);
      std::cout << x << ' ' << y << ' ' << color << '\n';
      ++step;
    }
  }
}
