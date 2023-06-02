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
        _map.resize(1, std::vector<map_block>(1, UNKNOWN));
        _map[0][0] = EMPTY;

        _finished = false;
    }

    void solve() {
        // update_map();

        // for (int i = 0; i < 5; ++i) {
        //     go_ahead();
        // }

        // for (auto i : _map) {
        //     for (auto j : i) {
        //         std::cerr << j << " ";
        //     }
        //     std::cerr << std::endl;
        // }

        // _map = {
        //     { VISITED, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE,
        //       OBSTACLE, OBSTACLE, OBSTACLE },
        //     { VISITED, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE,
        //       OBSTACLE, OBSTACLE, OBSTACLE },
        //     { OBSTACLE, VISITED, OBSTACLE, VISITED, OBSTACLE, OBSTACLE, OBSTACLE,
        //       OBSTACLE, OBSTACLE, OBSTACLE },
        //     { OBSTACLE, OBSTACLE, VISITED, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE,
        //       OBSTACLE, OBSTACLE, OBSTACLE },
        //     { OBSTACLE, OBSTACLE, VISITED, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE,
        //       OBSTACLE, OBSTACLE, OBSTACLE },
        //     { OBSTACLE, OBSTACLE, OBSTACLE, VISITED, OBSTACLE, OBSTACLE, VISITED,
        //       OBSTACLE, OBSTACLE, OBSTACLE },
        //     { OBSTACLE, OBSTACLE, OBSTACLE, VISITED, OBSTACLE, VISITED, OBSTACLE,
        //     VISITED,
        //       OBSTACLE, OBSTACLE },
        //     { OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, VISITED, OBSTACLE, OBSTACLE,
        //       OBSTACLE, VISITED, OBSTACLE },
        //     { OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE,
        //       OBSTACLE, VISITED, OBSTACLE },
        //     { OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE, OBSTACLE,
        //       OBSTACLE, OBSTACLE, EMPTY },
        // };

        // _cur_x = _cur_y = 0;

        // dfs_entry();

        while (!_finished) {
            // std::cerr << "cur: " << _cur_x << " " << _cur_y << std::endl;
            update_map();
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
    void go_ahead() {
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

        update_map();
    }

    void look_around() {
        int old_d = _cur_direction;

        for (int temp_d = old_d + 2; temp_d <= old_d + 6; temp_d++) {
            int d = temp_d % 8;
            if (_cur_y + _cur_view[d][3].second < 0
                || _cur_x + _cur_view[d][3].first < 0) {
                continue;
            }
            if (_map[_cur_y + _cur_view[d][3].second][_cur_x + _cur_view[d][3].first]
                == UNKNOWN) {
                int clockwise_delta     = (_cur_direction - d + 8) % 8;
                int anticlockwise_delta = (d - _cur_direction + 8) % 8;
                while (_cur_direction != d) {
                    if (clockwise_delta < anticlockwise_delta) {
                        turn_clockwise();
                    }
                    else {
                        turn_anticlockwise();
                    }
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

    void to_neighbour(int x, int y) {
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

        go_ahead();
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
            to_neighbour(next.self.first, next.self.second);
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

    void dfs_entry() {
        _path.clear();
        int max_x = 0;
        for (int i = 0; i < _map.size(); ++i) {
            max_x = std::max(max_x, int(_map[i].size()));
        }
        _vis.resize(_map.size());
        for (int i = 0; i < _vis.size(); ++i) {
            _vis[i].resize(max_x);
            for (int j = 0; j < max_x; ++j) {
                _vis[i][j] = 0;
            }
        }

        std::cout << "enter";

        if (dfs_body(_cur_x, _cur_y)) {
            while (!_path.empty()) {
                std::cout << "path: ";
                std::cout << "(" << _path.front().first << ", " << _path.front().second
                          << "), ";
                _path.pop_front();
            }
        }
    }

    bool dfs_body(int cur_x, int cur_y) {
        // std::cout << "enter " << cur_x << cur_y << std::endl;
        _vis[cur_y][cur_x] = 1;
        _path.push_back(std::make_pair(cur_x, cur_y));
        for (int x_offset = -1; x_offset <= 1; ++x_offset) {
            for (int y_offset = -1; y_offset <= 1; ++y_offset) {
                if (in_map(cur_x + x_offset, cur_y + y_offset)) {
                    // std::cout << "inmap " << cur_x + x_offset << cur_y + y_offset
                    // << std::endl;
                    if (!_vis[cur_y + y_offset][cur_x + x_offset]) {
                        if (_map[cur_y + y_offset][cur_x + x_offset] == VISITED) {
                            if (dfs_body(cur_x + x_offset, cur_y + y_offset))
                                return true;
                        }
                        if (_map[cur_y + y_offset][cur_x + x_offset] == EMPTY) {
                            _path.push_back(
                                std::make_pair(cur_x + x_offset, cur_y + y_offset));
                            return true;
                        }
                    }
                }
            }
        }
        _path.pop_back();
        return false;
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