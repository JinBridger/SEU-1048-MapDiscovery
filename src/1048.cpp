#include <iostream>
#include <string>
#include <vector>

class solver {
public:
    void tester() {
        auto res = get_view();
        for (int i = 0; i < 8; ++i) {
            std::cout << res[i] << " ";
        }
    }

private:
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

    int _cur_direction;
    int _cur_x, _cur_y;
    int _map_x, _map_y;
};

int main() {
    solver s;
    // s.tester();
    return 0;
}