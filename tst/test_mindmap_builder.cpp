#include <gtest/gtest.h>

#include "mindmap/mm.hpp"

void print_node(const mm::Node& node) {
  std::cout << node.level() << " " << node.content() << std::endl;
  for (const auto &child : node.children()) {
    print_node(*child.get());
  }
}

TEST(dummy, test_mindmap) {
  mm::MindMapBuilder builder;
  builder.add_node(0, "Main");
  builder.add_node(1, "child1");
  builder.add_node(2, "granchild");
  builder.add_node(1, "otherchild");

  auto mindmap = builder.build();
  ASSERT_FALSE(mindmap.root().children().empty());
  ASSERT_EQ(1, mindmap.root().children()[0]->children().size());
}
