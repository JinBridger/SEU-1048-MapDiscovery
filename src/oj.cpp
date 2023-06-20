/**
 * @file oj.cpp
 * @author SEU algorithm course group
 * @brief problem 1048 judge program provided by SEU algorithm course group
 *        url: http://8.218.127.172/problem.php?id=1048
 * @version 0.1
 * @date 2023-06-20
 *
 * @copyright Copyright (c) 2023 SEU algorithm course group
 *
 */

#include "../lib/testlib/testlib.h"

#include <fstream>
#include <iostream>
#include <vector>

#define pi 3.14159265
#define maxlen 100000
using namespace std;

int    row;     // 列
int    column;  // 行
int**  _map;    // 地图二维数组
int**  map_jug;
int    not_visit;
int    v[8];
int    t[6];
int    x;  // 用户坐标
int    y;
int    direction;
double length;
double total_length = 0;

void init() {

    not_visit = 0;
    column    = inf.readInt();
    row       = inf.readInt();
    _map      = new int*[column];
    map_jug   = new int*[column];
    int number;
    for (int i = 0; i < column; i++) {
        for (int j = 0; j < row; j++) {
            if (j == 0) {
                _map[i]    = new int[row];
                map_jug[i] = new int[row];
            }

            number        = inf.readInt();
            _map[i][j]    = number;
            map_jug[i][j] = number;
            if (number == 0)
                not_visit += 1;
        }
    }
    x             = 0;
    y             = 0;
    length        = 0;
    direction     = 0;
    map_jug[0][0] = 1;
    not_visit -= 1;
}

bool response() {
    length += 1;
    int idx = 0;
    int dx;
    int dy;
    for (int i = -1; i <= 1; i++) {
        double ddy = cos((direction + i) * pi / 4);
        double ddx = -sin((direction + i) * pi / 4);
        if (ddy >= 0)
            dy = ( int )(ddy + 0.5);
        else
            dy = ( int )(ddy - 0.5);
        if (ddx >= 0)
            dx = ( int )(ddx + 0.5);
        else
            dx = ( int )(ddx - 0.5);
        t[2 * i + 2] = dx;
        t[2 * i + 3] = dy;
        for (int j = 1; j <= 2; j++) {

            if (x + dx > column - 1 || y + dy > row - 1 || x + dx < 0 || y + dy < 0) {
                v[idx] = 2;
                idx++;
                if (j == 1) {
                    v[idx] = 2;
                    idx++;
                    break;
                }
            }
            else {
                v[idx] = _map[x + dx][y + dy];
                idx++;
                if (v[idx - 1] == 1 && j == 1) {
                    v[idx] = 2;
                    idx++;
                    break;
                }
                dx = dx * 2;
                dy = dy * 2;
            }
        }
        idx += 1;
    }

    if (v[0] == 0 && v[3] == 0) {
        dy = t[1] + t[3];
        dx = t[0] + t[2];

        if (x + dx > column - 1 || y + dy > row - 1 || x + dx < 0 || y + dy < 0)
            v[2] = 2;
        else
            v[2] = _map[x + dx][y + dy];
    }
    else
        v[2] = 2;
    if (v[6] == 0 && v[3] == 0) {
        dy = t[3] + t[5];
        dx = t[2] + t[4];

        if (x + dx > column - 1 || y + dy > row - 1 || x + dx < 0 || y + dy < 0)
            v[5] = 2;
        else
            v[5] = _map[x + dx][y + dy];
    }
    else
        v[5] = 2;

    cout << to_string(v[0]) + to_string(v[1]) + to_string(v[2]) + to_string(v[3])
                + to_string(v[4]) + to_string(v[5]) + to_string(v[6]) + to_string(v[7])
         << endl;
    return true;
}

bool turn(int a)  // 1逆时针 0顺时针
{
    if (a == 1) {
        direction = (direction + 1) % 8;
        length += 1;
    }
    if (a == 0) {
        direction = (direction + 7) % 8;
        length += 1;
    }
    return true;
}

bool move() {

    int    dx, dy;
    double ddy = cos(( direction )*pi / 4);
    double ddx = -sin(( direction )*pi / 4);
    if (ddy >= 0)
        dy = ( int )(ddy + 0.5);
    else
        dy = ( int )(ddy - 0.5);
    if (ddx >= 0)
        dx = ( int )(ddx + 0.5);
    else
        dx = ( int )(ddx - 0.5);

    if (x + dx < 0 || x + dx > column - 1 || y + dy < 0 || y + dy > row - 1) {
        cout << 0 << endl;
        return false;
    }
    else if (_map[x + dx][y + dy] == 0) {
        x = x + dx;
        y = y + dy;
        length += sqrt(dx * dx + dy * dy);
        if (map_jug[x][y] == 0) {
            not_visit--;
            map_jug[x][y] = 1;
        }
        if (not_visit == 0)
            cout << 1 << endl;
        else
            cout << 0 << endl;
        return true;
    }
    else {
        cout << 0 << endl;
        return false;
    }
}

int begin_task() {
    init();
    while (not_visit > 0) {

        if (length > maxlen)
            break;

        int action;
        int n = 0;

        action = ouf.readInt();

        switch (action) {
        case 0:
            turn(0);
            break;
        case 1:
            turn(1);
            break;
        case 2:
            move();
            break;
        case 3:
            response();
            break;
        default:
            break;
        }
    }

    for (int i = 0; i < column; i++) {
        delete _map[i];
        delete map_jug[i];
    }
    delete _map;
    delete map_jug;
    if (not_visit == 0)
        return 1;
    else
        return 0;
}

int main(int argc, char** argv) {
    // std::cerr << argv[0] << std::endl << argv[1] << std::endl;
    char* log_path = argv[3];
    argv[3]        = NULL;
    argc--;
    registerInteraction(argc, argv);
    // tout << "ssssssssssssssssss" << endl;

    FILE* fq;
    fq          = fopen(log_path, "w");
    int map_num = row = inf.readInt();
    cout << map_num << endl;
    fprintf(fq, "begin logging\n");
    fflush(fq);
    int finish_num = 0;
    for (int i = 0; i < map_num; i++) {
        int res = begin_task();
        if (res == 0)
            break;
        else {
            total_length += length;
            finish_num += 1;
        }
    }
    if (finish_num == map_num) {

        tout << "Accepted" << endl;
        tout << total_length << endl;
        tout << "0" << endl;
        // int temp = (int) length;
        quitf(_ok, "finish the visit with %f length", total_length);
    }

    else {
        tout << "not finish" << endl;
        tout << -1 << endl;
        tout << "0" << endl;
        quitf(_wa, "not finish visit");
    }

    fclose(fq);
    return 0;
}