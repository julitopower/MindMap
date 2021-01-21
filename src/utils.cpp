#include <cstdio>
#include <fstream>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <graphviz/gvcext.h>
#include <iostream>
#include <memory>
#include <string>

#include "mindmap/cmm.h"
#include "mindmap/mm.hpp"
#include "mindmap/utils.hpp"

// We use the C interface of flex/bison, and also defined the
// mindmap datastructures in C
extern "C" {
// Flex/Bison
#include "parser.h"

extern FILE *yyin;

// Global handler for mm::MindMapBuilder. This is referred to
// as extern in the grammar.y
MM_HDL mmap;
}

namespace {

/* Add surounding quotes to a string. Allocates new memory, and does
   not release the memory provided. It always returns the new allocated
   memory */
std::string quote(const std::string &str) {
  if (str[0] != '\"') {
    return "\"" + str + "\"";
  }
  return str;
}

/* Given a node, it recursively prints the entire tree
   as a Graphviz digraph, excluding header and footer */
void node_to_dot(std::ofstream &os, const mm::Node &node, bool master = false) {
  if (!node.children().empty()) {
    std::string content = quote(node.content());
    if (master) {
      os << content << " [peripheries=2, fontname=\"alial bold\"];\n";
    }
    for (const auto &child : node.children()) {
      std::string child_content = quote(child->content());

      os << content << " -> " << child_content;

      if (master) {
        os << "[color=red]";
      }

      os << std::endl;
      node_to_dot(os, *child.get());
    }
  }
}

void parse(const char *mmfilepath, mm::MindMapBuilder &mm_builder) {
  // Initialize the parsing machinery. Note that we MUST initialize the
  // mmap Gloval. This variable is internally used by the parser to build
  // the mindmap in-memory representation
  yyin = fopen(mmfilepath, "r");
  mmap = static_cast<MM_HDL>(&mm_builder);

  // Parse and as a print the resulting mindmap (this is temporary)
  // TODO: Error handling
  yyparse();
  fclose(yyin);
}
} // namespace

namespace mm {
extern "C" bool to_png(const char *dotfilepath, const char *pngfilepath) {
  // Get a context
  GVC_t *gvc = gvContext();
  // Read a file into a graph
  FILE *fin = fopen(dotfilepath, "r");
  Agraph_t *graph = agread(fin, 0);

  gvLayout(gvc, graph, "dot");
  gvRenderFilename(gvc, graph, "png", pngfilepath);
  gvFreeLayout(gvc, graph);
  agclose(graph);
  fclose(fin);

  gvFreeContext(gvc);
  return true;
}

extern "C" bool to_dot(const char *mmfilepath, const char *dotfilepath) {
  mm::MindMapBuilder mm_builder{};

  // Parse populates the builder
  parse(mmfilepath, mm_builder);
  MindMap mindmap{mm_builder.build()};

  std::ofstream ofs{dotfilepath};
  // Generate the output
  // write the header
  ofs << "digraph G {\n";
  ofs << "bgcolor=\"#ffffff\"\n";
  // Layout from left to right
  ofs << "rankdir=LR;\n";

  // global node attributes
  ofs << "graph [bgcolor=\"#ffcc40\", splines=curved, root=\"JustDoIt\", "
         "layout=dot, K=1.1,  overlap=false, start=42, maxiter=100000]\n";
  ofs << "node [shape=box, "
         "color=\"#2e6e3f\"fillcolor=\"#feffed\"style=\"filled,rounded\", "
         "fontname=arial, fontsize=11]\n";
  ofs << "edge [arrowhead = open, color=\"#002b36\"]\n";

  // Write out nodes

  node_to_dot(ofs, mindmap.root(), true);

  // Write footer
  ofs << "}\n";

  return true;
}
} // namespace mm
