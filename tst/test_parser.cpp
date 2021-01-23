#include <fstream>
#include <functional>
#include <gtest/gtest.h>

#include "mindmap/mm.hpp"
#include "mindmap/utils.hpp"

namespace mm {
extern int parse(const char *mmfilepath, mm::MindMapBuilder &mm_builder);
}

namespace {

const int PARSER_SUCESS = 0;
const int PARSER_FAILURE = 1;
const bool DOT_SUCESS = true;
const bool DOT_FAILURE = false;

struct TestFile;
void test_parser(TestFile &f, int expected);
void test_dot_generation(TestFile &f, bool expected);

// Utility to create and populate a test file
struct TestFile {
  const std::string filename_;
  const std::string filepath_;
  std::ofstream ofs;

  TestFile(const std::string &filename)
      : filename_{filename}, filepath_{"./data/" + filename}, ofs{filepath_} {}

  TestFile &operator<<(const std::string &line) {
    ofs << line;
    return *this;
  }

  void write() {
    ofs.flush();
    ofs.close();
  }

  void test(int expected_parser_return = PARSER_SUCESS,
            bool expected_generation_return = true) {
    write();
    test_parser(*this, expected_parser_return);
    test_dot_generation(*this, expected_generation_return);
  }

  const std::string &filepath() { return filepath_; }
};

void traverse(
    const mm::Node &node,
    std::function<void(const mm::Node &)> fn = [](const mm::Node &) {}) {
  fn(node);
  for (const auto &child : node.children()) {
    traverse(*child.get());
  }
}

void test_parser(TestFile &f, int expected) {
  // Test just the parser
  mm::MindMapBuilder mm_builder{};

  // Parse populates the builder
  ASSERT_EQ(expected, mm::parse(f.filepath().c_str(), mm_builder));
  mm::MindMap mindmap{mm_builder.build()};
  traverse(mindmap.root());
}

void test_dot_generation(TestFile &f, bool expected) {
  const std::string output{f.filepath() + ".dot"};
  ASSERT_EQ(expected, mm::to_dot(f.filepath().c_str(), output.c_str()));
}

} // namespace

TEST(parser, single_node) {
  TestFile f{"single_node.mm"};
  f << "* Main\n";
  f.test();
}

// TODO: This test is currently failing. The parser doen't catch
// semantic checks yet.
TEST(parser, incorrect_multi_node) {
  TestFile f{"multi_node.mm"};
  f << "* Main\n"
    << "** Child1\n"
    << "** Child2\n"
    << "**** Gran1\n"
    << "** Child3\n";
  f.test(PARSER_FAILURE, DOT_FAILURE);
}

