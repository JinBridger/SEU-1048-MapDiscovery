/**
 * @file bf.cpp
 * @author jinbridge (2635480475@qq.com)
 * @brief brute force program to solve "all_solution" array in 1048.cpp
 * @version 0.1
 * @date 2023-06-20
 *
 * @copyright Copyright (c) 2023 jinbridge. All Rights Reserved.
 *
 */

#include <cstdio>
#include <iostream>
#include <string>

int         cur_depth = 0x7fffffff;
std::string cur_sol   = "";

std::string visit_str(std::string str, int pos) {
    str[pos]           = '0';
    str[(pos + 1) % 8] = '0';
    str[(pos + 7) % 8] = '0';
    return str;
}

bool finish(std::string str) {
    for (int i = 0; i < 8; ++i) {
        if (str[i] == '1')
            return false;
    }
    return true;
}

int vis[8];

std::string Binary(int x) {
    std::string s = "";
    while (x) {
        if (x % 2 == 0)
            s = '0' + s;
        else
            s = '1' + s;
        x /= 2;
    }
    int l = s.length();
    if (l < 8) {
        for (int i = 0; i < 8 - l; ++i) {
            s = "0" + s;
        }
    }
    return s;
}

void dfs(std::string sol, std::string str, int dir, int depth) {
    if (finish(str)) {
        if (depth < cur_depth) {
            cur_depth = depth;
            cur_sol   = sol;
        }
        return;
    }

    if (depth >= 8) {
        return;
    }
    if (!vis[dir]) {
        vis[dir] = 1;
        dfs(sol + "3", visit_str(str, dir), dir, depth + 1);
        vis[dir] = 0;
    }
    dfs(sol + "0", str, (dir + 7) % 8, depth + 1);
    dfs(sol + "1", str, (dir + 1) % 8, depth + 1);
}

void reset() {
    cur_depth = 0x7fffffff;
    cur_sol   = "";
    for (int i = 0; i < 8; ++i) {
        vis[i] = 0;
    }
}

int main() {
    freopen("./src/sheet.cpp", "w", stdout);

    for (int i = 0; i <= 255; ++i) {
        // std::cout << Binary(i) << std::endl;
        dfs("", Binary(i), 0, 0);
        std::cout << "\"" << cur_sol << "\"," << std::endl;
        reset();
    }

    // dfs("", "11010011", 0, 0);
    // std::cout << cur_sol << std::endl;
}