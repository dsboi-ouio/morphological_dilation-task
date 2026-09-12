#include "pipeline.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

namespace {

void printUsage(std::ostream& output, const char* programName) {
    output << "Usage: " << programName << " <input-file>\n"
           << "Example: " << programName
           << " examples/input_12x12.txt\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        printUsage(std::cout, argv[0]);
        return 0;
    }

    if (argc != 2) {
        printUsage(std::cerr, argv[0]);
        return 2;
    }

    try {
        std::ifstream inputFile(argv[1]);
        if (!inputFile) {
            std::cerr << "Error: cannot open input file: " << argv[1] << '\n';
            return 1;
        }

        const morphology::DilationPipeline pipeline;
        pipeline.run(inputFile, std::cout);
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
