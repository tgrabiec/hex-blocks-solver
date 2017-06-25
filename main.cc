#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <experimental/optional>
#include <chrono>

namespace stdx = std::experimental;

using color_type = int;
const color_type board_color = 1;

struct point {
    int x;
    int y;
};

struct shape {
    std::vector<point> points; // sorted first by y, then by x.
    int width;
    int height;
    color_type color;
    std::vector<color_type> colors; // for painting another shapes over this shape

    shape(color_type color_, std::vector<point> points_)
        : points(std::move(points_))
        , color(color_)
    {
        width = 0;
        height = 0;
        for (auto&& p : points) {
            width = std::max(width, p.x + 1);
            height = std::max(height, p.y + 1);
        }
        colors.resize(width * height);
        for (auto&& p : points) {
            set(p.x, p.y, color);
        }
    }

    color_type get(int x, int y) const {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return 0;
        }
        return colors[y * width + x];
    }

    void set(int x, int y, color_type c) {
        colors[y * width + x] = c;
    }

    bool full() const {
        for (auto&& p : points) {
            if (get(p.x, p.y) == color) {
                return false;
            }
        }
        return true;
    }

    template<typename T, typename Func>
    T project(const shape& s, int x, int y, Func&& f, T def = {}) {
        for (auto&& p : s.points) {
            auto delta = x % 2 ? (p.x % 2) : 0;
            auto res = f(p.x, p.y, x + p.x, y + p.y - delta);
            if (res) {
                return *res;
            }
        }
        return def;
    }

    bool fits(const shape& s, int x, int y) {
        return project<bool>(s, x, y, [&] (int px, int py, int bx, int by) -> stdx::optional<bool> {
            if (get(bx, by) != color) {
                return false;
            }
            return stdx::nullopt;
        }, true);
    }

    // Call only when fits(s, x, y)
    void draw(const shape& s, int x, int y) {
        project<int>(s, x, y, [&] (int px, int py, int bx, int by) -> stdx::optional<int> {
            set(bx, by, s.color);
            return stdx::nullopt;
        });
    }

    friend std::ostream& operator<<(std::ostream& out, shape& s) {
        const int cell_width = 5;
        const int cell_height = 3;
        auto board_width = s.width * cell_width;
        char board[board_width*(s.height * cell_height + 1)];

        std::fill(&board[0], &board[0] + sizeof(board), ' ');

        auto draw_cell = [&] (int x, int y, char label) {
            int bx = x * cell_width;
            int by = y * cell_height + !(x % 2);
            char* p = &board[by * board_width + bx];
            *p++ = '.';
            *p++ = '-';
            *p++ = '-';
            *p++ = '-';
            *p++ = '.';
            p += board_width - cell_width;
            *p++ = '|';
            *p++ = ' ';
            *p++ = label;
            *p++ = ' ';
            *p++ = '|';
            p += board_width - cell_width;
            *p++ = '\'';
            *p++ = '_';
            *p++ = '_';
            *p++ = '_';
            *p = '\'';
        };

        auto cp = s.colors.begin();
        for (int y = 0; y < s.height; ++y) {
            for (int x = 0; x < s.width; ++x) {
                auto c = *cp++;
                if (c) {
                    if (c == board_color) {
                        draw_cell(x, y, ' ');
                    } else if (c < 10) {
                        draw_cell(x, y, char('0' + c));
                    } else {
                        draw_cell(x, y, char('A' + c - 10));
                    }
                }
            }
        }

        int i = 0;
        for (auto c : board) {
            out << c;
            if (++i % board_width == 0) {
                out << "\n";
            }
        }
        return out;
    }
};

bool solve(shape& board, std::vector<shape>& shapes, int& steps) {
    if (board.full()) {
        return true;
    }

    if (shapes.empty()) {
        return false;
    }

    shape s = shapes.back();
    shapes.pop_back();
    shape original = board;

    for (int y = 0; y < board.height - s.height + 1; y++) {
        for (int x = 0; x < board.width - s.width + 1; x++) {
            if (board.fits(s, x, y)) {
                board.draw(s, x, y);
                ++steps;
                if (solve(board, shapes, steps)) {
                    return true;
                }
                board = original;
            }
        }
    }

    auto res = solve(board, shapes, steps);
    shapes.push_back(std::move(s));
    return res;
}

int main() {
    color_type c = board_color;

    std::vector<point> board_points;
    for (int j = 0; j < 6; ++j) {
        for (int i = 0; i < 5; ++i) {
            board_points.emplace_back(point{i, j});
        }
    }
    shape board(c++, std::move(board_points));

    std::vector<shape> shapes;

    //
    // .---.
    // | 2 |
    // '___'
    //
    shapes.push_back(shape(c++, {
        {0, 0}
    }));

    //
    // .---.     .---.
    // | 3 |     | 3 |
    // '___'.---.'___'
    // .---.| 3 |.---.
    // | 3 |'___'| 3 |
    // '___'     '___'
    //
    shapes.push_back(shape(c++, {
        {0, 0},
        {2, 0},
        {0, 1},
        {1, 1},
        {2, 1}
    }));

    //
    // .---.
    // | 4 |
    // '___'.---.
    //      | 4 |
    //      '___'
    //
    //
    shapes.push_back(shape(c++, {
        {0, 0},
        {1, 1}
    }));

    //
    // .---.     .---.
    // | 5 |     | 5 |
    // '___'.---.'___'.---.
    //      | 5 |.---.| 5 |
    //      '___'| 5 |'___'
    //           '___'
    //
    shapes.push_back(shape(c++, {
        {0, 0},
        {2, 0},
        {1, 1},
        {2, 1},
        {3, 1}
    }));

    //
    // .---.
    // | 6 |
    // '___'.---.
    // .---.| 6 |.---.
    // | 6 |'___'| 6 |
    // '___'.---.'___'
    //      | 6 |
    //      '___'
    //
    shapes.push_back(shape(c++, {
        {0, 0},
        {0, 1},
        {1, 1},
        {2, 1},
        {1, 2}
    }));

    //
    // .---.
    // | 7 |
    // '___'
    // .---.
    // | 7 |
    // '___'
    // .---.
    // | 7 |
    // '___'
    //
    shapes.push_back(shape(c++, {
        {0, 0},
        {0, 1},
        {0, 2}
    }));

    //
    //           .---.
    //           | 8 |
    //      .---.'___'
    // .---.| 8 |
    // | 8 |'___'
    // '___'
    // .---.
    // | 8 |
    // '___'
    //
    shapes.push_back(shape(c++, {
        {2, 0},
        {0, 1},
        {1, 1},
        {0, 2}
    }));

    //      .---.
    // .---.| 9 |
    // | 9 |'___'
    // '___'.---.
    //      | 9 |.---.
    //      '___'| 9 |
    //           '___'
    //
    shapes.push_back(shape(c++, {
        {0, 0},
        {1, 0},
        {1, 1},
        {2, 1}
    }));

    //      .---.
    // .---.| A |
    // | A |'___'
    // '___'.---.
    //      | A |
    //      '___'
    //      .---.
    // .---.| A |
    // | A |'___'
    // '___'
    //
    shapes.push_back(shape(c++, {
        {0, 0},
        {1, 0},
        {1, 1},
        {0, 2},
        {1, 2}
    }));

    std::cout << "board:\n" << board << "\n";
    std::cout << "shapes:\n";
    for (auto&& s : shapes) {
        std::cout << s << "\n";
    }

    // Widest figures first
    std::sort(shapes.begin(), shapes.end(), [] (const shape& s1, const shape& s2) {
        return std::max(s1.width, s1.height) > std::max(s2.width, s2.height);
    });

    auto t1 = std::chrono::steady_clock::now();
    int steps = 0;

    bool solved = solve(board, shapes, steps);

    auto t2 = std::chrono::steady_clock::now();
    std::cout << "Performed " << steps << " steps in " << std::chrono::duration_cast<std::chrono::duration<float>>(t2 - t1).count() << " [s].\n";
    if (solved) {
        std::cout << "Solution found:\n" << board << "\n";
        if (!shapes.empty()) {
            std::cout << "Unused shapes:\n";
            for (auto&& s : shapes) {
                std::cout << s << "\n";
            }
        }
        return 0;
    } else {
        std::cout << "Solution does not exist.\n";
        return 1;
    }
}
