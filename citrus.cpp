#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Type: " << argv[0] << " -h\n";
        std::cout << "For help on usage.\n";
        return 1;
    }

    std::vector<std::string> args(argc);

    for (int i{}; i < argc; i++)
        args.emplace_back(argv[i]);

    for (int i{1}; i < argc; i++) {
        if (args[i] == "-h\0") {
            std::cout << "Usage: " << args[0] << " [options]\n\n";
            std::cout << "Options:\n";
            std::cout << "  -h         Display help information.\n";
            std::cout << "  -v         Display compiler version\n";
            std::cout << "  -bld       Build project without running or debuging.\n";
            std::cout << "  -run       Build and run project without debuging.\n";
            std::cout << "  -dbg       Build and debug project.\n";
            std::cout << "  -o <file>  Place the output file into <file>.\n";
            return 0;
        }
    }
}