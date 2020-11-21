#include <iostream>
#include <pgs++/pgs.hpp>

int main() {
    Pgs::Subtitle subtitle;

    const auto &time = subtitle.getPresentationTime() == 0u;

    std::cout << std::to_string(time) << std::endl;

    return EXIT_SUCCESS;
}
