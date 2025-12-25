#include "parser/bencoding.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    parser::Bvalue str = "hello\n";
    parser::Bitem it(str);

    std::cout << std::get<std::string>(it.val);
    return 0;
}
