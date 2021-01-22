#include <fstream>
#include <gtest/gtest.h>

#include "mindmap/mm.hpp"
#include "mindmap/utils.hpp"

namespace mm {
extern int parse(const char *mmfilepath, mm::MindMapBuilder &mm_builder);
}

// Utility to create and populate a test file
struct TestFile {
  const std::string& filename;
  std::ofstream ofs;
  TestFile(const std::string& filename) : filename {filename} , ofs{"./data/" + filename} {}

  TestFile& operator<<(const std::string& line) {
    ofs << line;
    return *this;
  }

  void write() {
    ofs.flush();
    ofs.close();
  }

  std::string filepath() {
    // TODO: Cache the value on construction and return a reference to it
    return "./data/" + filename;
  }
  
};

TEST(parser, single_node) {
  TestFile f{"single_node.mm"};
  f << "* Main\n";
  f.write();

  // TODO: Fix once the TEstFile returns a reference
  const std::string input {f.filepath()};
  const std::string output {f.filepath() + ".dot"};  
  
  {
    // parser test
    mm::MindMapBuilder mm_builder{};

    // Parse populates the builder
    ASSERT_EQ(0, mm::parse(input.c_str(), mm_builder));
    mm::MindMap mindmap{mm_builder.build()};
    std::cout << mindmap.root().content() << std::endl;
  }

  {
    ASSERT_TRUE(mm::to_dot(input.c_str(), output.c_str()));
  }
}
