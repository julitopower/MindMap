#include <iostream>
#include <string>

#include "mindmap/utils.hpp"

namespace {
inline void p(const char* arg) {
  printf("%s\n", arg);
}

void print_help() {
  p("dotpng input_file");
  p("");
  p("  input_file: required argument. Path to input file. Must be a");
  p("              graphviz dot file.");
}
} // unnamed namespace

int main(int argc, char** argv)
{
  // We need for the moment just one input argument
  if (argc != 2) {
    print_help();
    exit(1);
  }

  // build output filepath as input.png
  std::string output_filepath{argv[1]};
  output_filepath += ".png";
  mm::to_png(argv[1], output_filepath.c_str());
  return 0;
}
