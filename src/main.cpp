#include "bencoding/bencoding.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    bencoding::Bvalue str = "hello\n";
    bencoding::Bitem it(str);

    std::cout << std::get<std::string>(it.val);
    return 0;
}
