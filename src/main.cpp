#include "parsing/bencoding.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    parsing::bencoding::Bvalue str = "hello\n";
    parsing::bencoding::Bitem it(str);

    std::cout << std::get<std::string>(it.val);
    return 0;
}
