#include <iostream>
#include <string>

#include "mindmap/utils.hpp"

namespace {
inline void p(const char *arg) { printf("%s\n", arg); }

void print_help() {
  p("mmpng input_file");
  p("");
  p("  input_file: required argument. Path to input file. Must be a");
  p("              mindmap file.");
}
} // unnamed namespace

int main(int argc, char **argv) {
  // We need for the moment just one input argument
  if (argc != 2) {
    print_help();
    exit(1);
  }

  // build output filepath as input.png
  std::string output_filepath{argv[1]};
  output_filepath += ".dot";
  if(mm::to_dot(argv[1], output_filepath.c_str())) {
    return 1;
  }

  std::string png_filepath{argv[1]};
  png_filepath += ".png";
  mm::to_png(output_filepath.c_str(), png_filepath.c_str());

  return 0;
}
