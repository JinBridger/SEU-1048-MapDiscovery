#include <iostream>
#include <queue>
#include <string>
#include <vector>

class solver {
public:
    solver() {
        _cur_direction = E;
        _cur_x         = 0;
        _cur_y         = 0;
        // _map_x = 0;
        // _map_y = 0;
        _map.resize(1, std::vector<map_block>(1, UNKNOWN));
        _map[0][0] = EMPTY;
    }

    void tester() {
        update_map();

        for (int i = 0; i < 5; ++i) {
            go_ahead();
        }

        for (auto i : _map) {
            for (auto j : i) {
                std::cerr << j << " ";
            }
            std::cerr << std::endl;
        }
    }

private:
    void go_ahead() {
        std::cout << "2" << std::endl;
        int a;
        std::cin >> a;

        switch (_cur_direction) {
        case E:
            _cur_x += 1;
            break;
        case NE:
            _cur_x += 1;
            _cur_y -= 1;
            break;
        case N:
            _cur_y -= 1;
            break;
        case NW:
            _cur_x -= 1;
            _cur_y -= 1;
            break;
        case W:
            _cur_x -= 1;
            break;
        case SW:
            _cur_x -= 1;
            _cur_y += 1;
            break;
        case S:
            _cur_y += 1;
            break;
        case SE:
            _cur_x += 1;
            _cur_y += 1;
            break;
        }

        update_map();
    }

    std::vector<int> get_view() {
        std::cout << 3 << std::endl;
        std::string str;
        std::cin >> str;
        std::vector<int> view;
        for (int i = 0; i < 8; ++i) {
            view.push_back(str[i] - '0');
        }
        return view;
    }

    void update_map() {
        std::vector<int>                 view     = get_view();
        std::vector<std::pair<int, int>> view_pos = get_view_pos();
        for (int i = 0; i < 8; ++i) {
            int x = view_pos[i].first;
            int y = view_pos[i].second;
            if ((x < 0 || y < 0)) {
                continue;
            }

            if (y >= _map.size())
                _map.resize(y + 1, std::vector<map_block>(x + 1, UNKNOWN));
            if (x >= _map[y].size())
                _map[y].resize(x + 1, UNKNOWN);

            if ((i == 0 || i == 3 || i == 6) && view[i] == 2) {
                _map[y][x] = BORDER;
            }
            if ((i == 1 || i == 4 || i == 7) && view[i] == 2 && view[i - 1] == 0) {
                _map[y][x] = BORDER;
            }
            if ((i == 2 || i == 5) && view[i] == 2 && view[i - 2] == 0
                && view[i + 1] == 0) {
                _map[y][x] = BORDER;
            }

            if (_map[y][x] != UNKNOWN)
                continue;

            if (view[i] == 0) {
                _map[y][x] = EMPTY;
            } else if (view[i] == 1) {
                _map[y][x] = OBSTACLE;
            } else if (view[i] == 2) {
                _map[y][x] = UNKNOWN;
            }
        }
    }

    std::vector<std::pair<int, int>> get_view_pos() {
        std::vector<std::pair<int, int>> view_pos = _cur_view[_cur_direction];
        for (int i = 0; i < 8; ++i) {
            view_pos[i].first += _cur_x;
            view_pos[i].second += _cur_y;
        }
        return view_pos;
    }

    typedef enum map_block {
        EMPTY    = 0,
        OBSTACLE = 1,
        UNKNOWN  = 2,
        BORDER   = 3,
        VISITED  = 4
    } map_block;

    typedef enum direction {
        E  = 0,
        NE = 1,
        N  = 2,
        NW = 3,
        W  = 4,
        SW = 5,
        S  = 6,
        SE = 7
    } direction;

    direction _cur_direction;

    // clang-format off
    std::vector<std::vector<std::pair<int, int>>> _cur_view = {
        {{1, 1},  {2, 2},  {2, 1},  {1, 0},   {2, 0},   {2, -1},   {1, -1},   {2, -2}},
        {{1, 0},   {2, 0},   {2, -1},   {1, -1},   {2, -2},   {1, -2},   {0, -1},   {0, -2}},
        {{1, -1},   {2, -2},   {1, -2},   {0, -1},   {0, -2},   {-1, -2},  {-1, -1},  {-2, -2}},
        {{0, -1},   {0, -2},   {-1, -2},  {-1, -1},  {-2, -2},  {-2, -1},  {-1, 0},  {-2, 0}},
        {{-1, -1},  {-2, -2},  {-2, -1},  {-1, 0},  {-2, 0},  {-2, 1}, {-1, 1}, {-2, 2}},
        {{-1, 0},  {-2, 0},  {-2, 1}, {-1, 1}, {-2, 2}, {-1, 2}, {0, 1},  {0, 2}},
        {{-1, 1}, {-2, 2}, {-1, 2}, {0, 1},  {0, 2},  {1, 2},  {1, 1},  {2, 2}},
        {{0, 1},  {0, 2},  {1, 2},  {1, 1},  {2, 2},  {2, 1},  {1, 0},   {2, 0}}
    };
    // clang-format on

    int _cur_x, _cur_y;
    // int _map_x, _map_y;

    std::vector<std::vector<map_block>> _map;
};

int main() {
    int i;
    std::cin >> i;
    while (i--) {
        solver s;
        s.tester();
    }
    // solver s;
    // s.tester();
    return 0;
}