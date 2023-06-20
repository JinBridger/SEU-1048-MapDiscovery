/**
 * @file 1048.cpp
 * @author jinbridge (2635480475@qq.com), idawnlight (idawn@live.com)
 * @brief a simple solution of problem 1048 in http://8.218.127.172/problem.php?id=1048
 *        the online judge of algorithm course in Southeast University,
 *        this code gets 13629.8 pts.
 * @version 13629.8
 * @date 2023-06-20
 *
 * @copyright Copyright (c) 2023 jinbridge and idawnlight. All Rights Reserved.
 *
 */

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_set>
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
        while (!_finished) {
            _map[_cur_y][_cur_x] = VISITED;
            // firstly, get what 8 surrounding blocks are
            look_around();

            // then check if there's an unvisited block on these directions
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
                    // if there is a block that hasn't been visited, go there
                    to_neighbour(_cur_x + _cur_view[d][3].first,
                                 _cur_y + _cur_view[d][3].second);
                    flag = false;
                    break;
                }
            }

            // if all 8 surrounding blocks are visited, search an unvisited block to go
            if (flag)
                to_next_empty();
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
    }

    // check if a block is both in the map and unknown
    bool in_map_and_unknown(int x, int y) {
        if (in_map(x, y)) {
            return _map[y][x] == UNKNOWN;
        }
        return false;
    }

    // execute solutions in all_solution
    void execute_str(std::string str) {
        for (int i = 0; i < str.length(); ++i) {
            if (str[i] == '0') {
                turn_clockwise();
            }
            if (str[i] == '1') {
                turn_anticlockwise();
            }
            if (str[i] == '3') {
                update_map();
            }
        }
    }

    void look_around() {
        int index = 0;
        int old_d = _cur_direction;
        // get the surrounding blocks' status
        for (int tmp_d = old_d; tmp_d <= old_d + 7; ++tmp_d) {
            int d  = tmp_d % 8;
            int nx = _cur_x + _cur_view[d][3].first;
            int ny = _cur_y + _cur_view[d][3].second;
            if (in_map(nx, ny) && _map[ny][nx] == UNKNOWN) {
                index += 1;
            }
            index *= 2;
        }
        index /= 2;
        // get the solution and execute it!
        std::string sol = all_solution[index];
        execute_str(sol);
    }

    void turn_clockwise() {
        std::cout << 0 << std::endl;
        _cur_direction = direction((_cur_direction + 8 - 1) % 8);
    }

    void turn_anticlockwise() {
        std::cout << 1 << std::endl;
        _cur_direction = direction((_cur_direction + 1) % 8);
    }

    // go to a neighbour block
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

    // use BFS to solve a path to an unvisited block
    void to_next_empty() {
        // copy the map to keep the original map unmodified
        auto             temp_map = _map;
        std::queue<step> q;
        std::stack<step> s;
        // a step stores the last coord and the next coord
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
        // found an unvisited block
        // now get the path
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

        // move according to the path
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

    // observe
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

    // update the map according to the observe result
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

    // get the coord of the observe result
    std::vector<std::pair<int, int>> get_view_pos() {
        std::vector<std::pair<int, int>> view_pos = _cur_view[_cur_direction];
        for (int i = 0; i < 8; ++i) {
            view_pos[i].first += _cur_x;
            view_pos[i].second += _cur_y;
        }
        return view_pos;
    }

    // check whether a block is in the map
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
        std::pair<int, int> parent;  // the last coord
        std::pair<int, int> self;    // current coord
    };

    direction _cur_direction;

    // clang-format off

    // coordinate offset of every blocks in a view
    // the first dimension is direction (the same as enum direction)
    // the second dimension is the sequence of the block in the direction
    // pair stores the coordinate offset {dx, dy}
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

    // the shortest solution of every situation
    // the index is the binary of the situation
    // e.g. assume 8 blocks around the block is 00011100=28
    // that means the NW, W, SW directions are unknown and other directions are all known
    std::string all_solution[256] = {
    "",      "3",     "03",       "03",       "003",    "003",     "003",      "003",
    "0003",  "30003", "0003",     "30003",    "0003",   "30003",   "0003",     "30003",
    "113",   "3113",  "030003",   "030003",   "00003",  "300003",  "030003",   "030003",
    "1113",  "31113", "030003",   "030003",   "00003",  "300003",  "030003",   "030003",
    "13",    "313",   "03113",    "03113",    "130003", "130003",  "130003",   "130003",
    "1113",  "31113", "0300003",  "0300003",  "131113", "3131113", "0030003",  "0030003",
    "113",   "3113",  "031113",   "031113",   "131113", "3131113", "0030003",  "0030003",
    "1113",  "31113", "0300003",  "0300003",  "131113", "3131113", "0030003",  "0030003",
    "3",     "3",     "303",      "303",      "3003",   "3003",    "3003",     "3003",
    "30003", "30003", "30003",    "30003",    "30003",  "30003",   "30003",    "30003",
    "113",   "3113",  "031113",   "031113",   "300003", "300003",  "3030003",  "3030003",
    "31113", "31113", "3030003",  "3030003",  "300003", "300003",  "3030003",  "3030003",
    "13",    "313",   "03113",    "03113",    "130003", "130003",  "130003",   "130003",
    "31113", "31113", "1300003",  "30300003", "131113", "3131113", "1300003",  "30030003",
    "113",   "3113",  "031113",   "031113",   "131113", "3131113", "1131113",  "30030003",
    "31113", "31113", "1131113",  "30300003", "131113", "3131113", "1131113",  "30030003",
    "3",     "3",     "03",       "03",       "3003",   "3003",    "3003",     "3003",
    "30003", "30003", "30003",    "30003",    "30003",  "30003",   "30003",    "30003",
    "3113",  "3113",  "030003",   "030003",   "300003", "300003",  "030003",   "030003",
    "31113", "31113", "030003",   "030003",   "300003", "300003",  "030003",   "030003",
    "13",    "313",   "03113",    "03113",    "130003", "130003",  "130003",   "130003",
    "31113", "31113", "0300003",  "0300003",  "131113", "3131113", "1300003",  "30030003",
    "3113",  "3113",  "031113",   "031113",   "131113", "3131113", "30030003", "30030003",
    "31113", "31113", "0300003",  "0300003",  "131113", "3131113", "30030003", "30030003",
    "3",     "3",     "303",      "303",      "3003",   "3003",    "3003",     "3003",
    "30003", "30003", "30003",    "30003",    "30003",  "30003",   "30003",    "30003",
    "3113",  "3113",  "031113",   "031113",   "300003", "300003",  "3030003",  "3030003",
    "31113", "31113", "3030003",  "3030003",  "300003", "300003",  "3030003",  "3030003",
    "13",    "313",   "03113",    "03113",    "130003", "130003",  "130003",   "130003",
    "31113", "31113", "1300003",  "30300003", "131113", "3131113", "1300003",  "30030003",
    "3113",  "3113",  "031113",   "031113",   "131113", "3131113", "30030003", "30030003",
    "31113", "31113", "30300003", "30300003", "131113", "3131113", "30030003", "30030003",
};
    // clang-format on

    int  _cur_x, _cur_y;  // the current coordinate
    bool _finished;

    std::vector<std::vector<map_block>> _map;

    std::deque<std::pair<int, int>> _path;  // temp var used in BFS
    std::vector<std::vector<int>>   _vis;   // temp var used in BFS
};

int main() {
    int i;
    std::cin >> i;
    while (i--) {
        solver s;
        s.solve();  // solve it!
    }
    return 0;
}