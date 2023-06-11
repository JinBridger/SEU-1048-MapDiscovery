#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <utility>
#include <vector>

class solver {
public:
    solver() {
        _cur_direction = E;
        _cur_x         = 0;
        _cur_y         = 0;
        // _map_x = 0;
        // _map_y = 0;
        _map.resize(60, std::vector<map_block>(60, UNKNOWN));
        _map[0][0] = EMPTY;

        _finished = false;
    }

    void solve() {
        // update_map();

        while (!_finished) {
            // std::cerr << "cur: " << _cur_x << " " << _cur_y << std::endl;
            _map[_cur_y][_cur_x] = VISITED;
            look_around();

            int  delta_direction[] = { 0, 1, -1, 2, -2, 3, -3, 4 };
            bool flag              = true;
            for (auto delta : delta_direction) {
                int d = (_cur_direction + delta + 8) % 8;
                if (_cur_y + _cur_view[d][3].second < 0
                    || _cur_x + _cur_view[d][3].first < 0) {
                    continue;
                }
                if (_map[_cur_y + _cur_view[d][3].second][_cur_x + _cur_view[d][3].first]
                    == EMPTY) {
                    to_neighbour(_cur_x + _cur_view[d][3].first,
                                 _cur_y + _cur_view[d][3].second);
                    flag = false;
                    break;
                }
            }

            if (flag)
                to_next_empty();

            // go_ahead();
        }
    }

private:
    void go_ahead(bool update = true) {
        std::cout << "2" << std::endl;
        int a;
        std::cin >> a;
        if (a == 1) {
            _finished = true;
            return;
        }

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

        if (update)
            ;
        // update_map();
    }

    bool in_map_and_unknown(int x, int y) {
        if (in_map(x, y)) {
            return _map[y][x] == UNKNOWN;
        }
        return false;
    }

    void look_around() {
        if (in_map_and_unknown(_cur_x + _cur_view[_cur_direction][3].first,
                               _cur_y + _cur_view[_cur_direction][3].second)
            || in_map_and_unknown(_cur_x + _cur_view[_cur_direction][0].first,
                                  _cur_y + _cur_view[_cur_direction][0].second)
            || in_map_and_unknown(_cur_x + _cur_view[_cur_direction][6].first,
                                  _cur_y + _cur_view[_cur_direction][6].second)) {
            update_map();
        }

        int old_d = _cur_direction;

        std::vector<int> unk_blocks;

        for (int temp_d = old_d + 2; temp_d <= old_d + 6; temp_d++) {
            int d = temp_d % 8;
            if (_cur_y + _cur_view[d][3].second < 0
                || _cur_x + _cur_view[d][3].first < 0) {
                continue;
            }
            if (_cur_y + _cur_view[d][3].second >= _map.size()
                || _cur_x + _cur_view[d][3].first
                       >= _map[_cur_y + _cur_view[d][3].second].size()) {
                continue;
            }
            if (_map[_cur_y + _cur_view[d][3].second][_cur_x + _cur_view[d][3].first]
                == UNKNOWN) {
                unk_blocks.push_back((d - old_d + 8) % 8);
                // int clockwise_delta     = (_cur_direction - d + 8) % 8;
                // int anticlockwise_delta = (d - _cur_direction + 8) % 8;
                // while (_cur_direction != d) {
                //     if (clockwise_delta < anticlockwise_delta) {
                //         turn_clockwise();
                //     }
                //     else {
                //         turn_anticlockwise();
                //     }
                // }
                // update_map();
            }
        }
        if (unk_blocks.empty())
            return;

        std::sort(unk_blocks.begin(), unk_blocks.end());

        // for (auto itm : unk_blocks) {
        //     std::cerr << itm << " ";
        // }
        // std::cerr << std::endl;

        int max_v = *unk_blocks.rbegin();
        int min_v = *unk_blocks.begin();

        int cost_clockwise     = 0;
        int cost_anticlockwise = 0;

        int max_turn_clockwise     = 7 - min_v;
        int max_turn_anticlockwise = max_v - 1;

        // std::cerr << "max_turn_clockwise: " << max_turn_clockwise << std::endl;
        // std::cerr << "max_turn_anticlockwise: " << max_turn_anticlockwise << std::endl;

        cost_clockwise = max_turn_clockwise + (max_turn_clockwise >= 4 ? 2 : 1);
        cost_anticlockwise =
            max_turn_anticlockwise + (max_turn_anticlockwise >= 4 ? 2 : 1);

        // std::cerr << "cost_clockwise: " << cost_clockwise << std::endl;
        // std::cerr << "cost_anticlockwise: " << cost_anticlockwise << std::endl;

        if (cost_anticlockwise > cost_clockwise) {
            if (max_turn_clockwise >= 4) {
                for (int i = 0; i < max_turn_clockwise - 3; ++i) {
                    turn_clockwise();
                }
                update_map();
                for (int i = max_turn_clockwise - 3; i < max_turn_clockwise; ++i) {
                    turn_clockwise();
                }
                update_map();
            }
            else {
                for (int i = 0; i < max_turn_clockwise; ++i) {
                    turn_clockwise();
                }
                update_map();
            }
        }
        else {
            if (max_turn_anticlockwise >= 4) {
                for (int i = 0; i < max_turn_anticlockwise - 3; ++i) {
                    turn_anticlockwise();
                }
                update_map();
                for (int i = max_turn_anticlockwise - 3; i < max_turn_anticlockwise;
                     ++i) {
                    turn_anticlockwise();
                }
                update_map();
            }
            else {
                for (int i = 0; i < max_turn_anticlockwise; ++i) {
                    turn_anticlockwise();
                }
                update_map();
            }
        }
    }

    void turn_clockwise() {
        std::cout << 0 << std::endl;
        _cur_direction = direction((_cur_direction + 8 - 1) % 8);
    }

    void turn_anticlockwise() {
        std::cout << 1 << std::endl;
        _cur_direction = direction((_cur_direction + 1) % 8);
    }

    void to_neighbour(int x, int y, bool update = true) {
        int dx = x - _cur_x;
        int dy = y - _cur_y;
        int new_d;
        for (new_d = 0; new_d < 8; new_d++) {
            if (_cur_view[new_d][3].first == dx && _cur_view[new_d][3].second == dy) {
                break;
            }
        }

        int clockwise_delta     = (_cur_direction - new_d + 8) % 8;
        int anticlockwise_delta = (new_d - _cur_direction + 8) % 8;

        while (_cur_direction != new_d) {
            if (clockwise_delta < anticlockwise_delta) {
                turn_clockwise();
            }
            else {
                turn_anticlockwise();
            }
        }

        go_ahead(update);
    }

    void to_next_empty() {
        auto             temp_map = _map;
        std::queue<step> q;
        std::stack<step> s;
        q.push({ { -1, -1 }, { _cur_x, _cur_y } });
        temp_map[_cur_y][_cur_x] = TRACED;
        while (!q.empty()) {
            auto cur = q.front();
            q.pop();
            s.push(cur);
            for (int i = 0; i < 8; ++i) {
                int x = cur.self.first + _cur_view[i][3].first;
                int y = cur.self.second + _cur_view[i][3].second;
                if (x < 0 || y < 0 || y >= temp_map.size() || x >= temp_map[y].size()) {
                    continue;
                }
                if (temp_map[y][x] == EMPTY) {
                    step new_step = { { cur.self.first, cur.self.second }, { x, y } };
                    s.push(new_step);
                    goto found;
                }
                else if (_map[y][x] == VISITED && temp_map[y][x] != TRACED) {
                    temp_map[y][x] = TRACED;
                    q.push({ { cur.self.first, cur.self.second }, { x, y } });
                }
            }
        }

    found:
        std::stack<step> path;
        auto             cur = s.top();
        s.pop();
        path.push(cur);
        while (!s.empty()) {
            auto next = s.top();
            s.pop();
            while (!(next.self.first == cur.parent.first
                     && next.self.second == cur.parent.second)) {
                next = s.top();
                s.pop();
            }
            path.push(next);
            cur = next;
        }

        path.pop();
        while (!path.empty()) {
            auto next = path.top();
            path.pop();
            if (_map[next.self.second][next.self.first] == VISITED) {
                to_neighbour(next.self.first, next.self.second, false);
            }
            else {
                to_neighbour(next.self.first, next.self.second, true);
            }
        }
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
            }
            else if (view[i] == 1) {
                _map[y][x] = OBSTACLE;
            }
            else if (view[i] == 2) {
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

    bool in_map(int x, int y) {
        if (0 <= y && y < _map.size())
            if (0 <= x && x < _map[y].size())
                return true;

        return false;
    }

    typedef enum map_block {
        EMPTY    = 0,
        OBSTACLE = 1,
        UNKNOWN  = 2,
        BORDER   = 3,
        VISITED  = 4,
        TRACED   = 5
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

    struct step {
        std::pair<int, int> parent;
        std::pair<int, int> self;
    };

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
    bool _finished;

    std::vector<std::vector<map_block>> _map;

    std::deque<std::pair<int, int>> _path;
    std::vector<std::vector<int>>   _vis;
};

int main() {
    int i;
    std::cin >> i;
    while (i--) {
        solver s;
        s.solve();
    }
    // solver s;
    // s.tester();
    return 0;
}