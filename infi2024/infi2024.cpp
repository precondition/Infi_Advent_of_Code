#include <array>
#include <cassert>
#include <fstream>
#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <istream>

constexpr static int max_dim{30}; // Exclusive upperbound

template<typename T, std::size_t N>
using Array3D = std::array<std::array<std::array<T, N>, N>, N>;

long compute_snow_in_block(long x, long y, long z, const std::vector<std::string> &instructions) {
    std::stack<long> stack{};

    const ssize_t instructions_size{static_cast<ssize_t>(instructions.size())};

    for (long pgm_cntr = 0; pgm_cntr < instructions_size; pgm_cntr++) {
        std::istringstream instruction{instructions.at(pgm_cntr)};

        std::string opcode{};
        instruction >> opcode;

        if (opcode == "push") {
            std::string operand{};
            instruction >> operand;
            if (operand == "X") {
                stack.push(x);
            } else if (operand == "Y") {
                stack.push(y);
            } else if (operand == "Z") {
                stack.push(z);
            } else {
                stack.push(std::stol(operand));
            }
        } else if (opcode == "add") {
            const long a{stack.top()};
            stack.pop();
            const long b{stack.top()};
            stack.pop();

            stack.push(a + b);
        } else if (opcode == "jmpos") {
            long operand{0};
            instruction >> operand;

            if (stack.top() >= 0) {
                pgm_cntr += operand;
            }
            stack.pop(); // jmpos unconditionally consumes an element off the stack
        } else if (opcode == "ret") {
            return stack.top();
        } else {
            throw std::invalid_argument("Invalid opcode");
        }
    }

    std::cerr << "Warning: No `ret` operation found!" << "\n";
    return stack.top();
}

constexpr std::array<std::array<int, 3>, 6> neighbor_offsets = {
    {
        {-1, 0, 0},
        {+1, 0, 0},
        {0, -1, 0},
        {0, +1, 0},
        {0, 0, -1},
        {0, 0, +1}
    }
};

void flood_fill(long x, long y, long z, long cloud_formation_id,
                Array3D<bool, max_dim> &is_cloudy, Array3D<long, max_dim> &cloud_formations) {
    // Invalid coordinates
    if (!(0 <= x && x < cloud_formations.size()
          && 0 <= y && y < cloud_formations.size()
          && 0 <= z && z < cloud_formations.size())) {
        return;
    }

    if (!is_cloudy[x][y][z]) {
        return;
    }

    if (cloud_formations[x][y][z] > 0) {
        if (cloud_formations[x][y][z] != cloud_formation_id) {
            std::cerr << "Cloud №" << cloud_formation_id << " bumped into cloud №" << cloud_formations[x][y][z] << "\n";
            assert(false);
        }
        return;
    }

    cloud_formations[x][y][z] = cloud_formation_id;

    for (std::array<int, 3> delta: neighbor_offsets) {
        // Tag neighbor with the same cloud formation ID
        flood_fill(x + delta[0], y + delta[1], z + delta[2], cloud_formation_id,
                   is_cloudy, cloud_formations);
    }
}

int main() {
    std::ifstream inputf{"input.txt"};
    if (!inputf.good()) {
        std::cerr << "Error while opening input.txt!" << "\n";
        return 1;
    }

    std::vector<std::string> instructions{};
    std::string instruction_line{};

    while (std::getline(inputf, instruction_line)) {
        instructions.push_back(instruction_line);
    }
    assert(!instructions.empty());

    long total_snow{0};

    // 3D boolean array determining if an area is cloudy, as defined by a snow quantity > 0 in that block
    Array3D<bool, max_dim> is_cloudy{};

    for (long z = 0; z < max_dim; ++z) {
        for (long y = 0; y < max_dim; ++y) {
            for (long x = 0; x < max_dim; ++x) {
                long snow_in_block{compute_snow_in_block(x, y, z, instructions)};
                assert(snow_in_block >= 0);

                total_snow += snow_in_block;

                if (snow_in_block > 0) {
                    is_cloudy[x][y][z] = true;
                }
            }
        }
    }


    std::cout << "[Part 1] Total amount of snow in the world is: " << total_snow << "\n";


    Array3D<long, max_dim> cloud_formations{};
    long n_cloud_formations{0};

    for (long z = 0; z < max_dim; ++z) {
        for (long y = 0; y < max_dim; ++y) {
            for (long x = 0; x < max_dim; ++x) {
                if (!is_cloudy[x][y][z]) {
                    continue;
                }

                if (cloud_formations[x][y][z] > 0) {
                    continue; // Already part of a cloud formation we know
                }

                ++n_cloud_formations;
                flood_fill(x, y, z, n_cloud_formations, is_cloudy, cloud_formations);
            }
        }
    }

    std::cout << "[Part 2] Total amount of cloud formations: " << n_cloud_formations << "\n";
}
