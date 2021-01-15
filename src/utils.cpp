#include <cstdio>
#include <fstream>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <graphviz/gvcext.h>
#include <string>

#include "mindmap/utils.hpp"

// We use the C interface of flex/bison, and also defined the
// mindmap datastructures in C
extern "C" {
// Mindmap data structures
#include "mindmap/cmm.h"
// Flex/Bison  
#include "parser.h"

extern FILE *yyin;
extern void *mmap;
}



namespace {

void node_to_dot(std::ofstream &os, Node *node) {
  if (node->nchildren > 0) {
    char *content = node->content;
    for (auto i = 0; i < node->nchildren; ++i) {
      os << content << " -> " << node->children[i]->content << std::endl;
      node_to_dot(os, node->children[i]);
    }
  }
}

void parse(const char* mmfilepath) {
  // Initialize the parsing machinery. Note that we MUST initialize the
  // mmap Gloval. This variable is internally used by the parser to build
  // the mindmap in-memory representation
  yyin = fopen(mmfilepath, "r");
  mmap = mm_new();

  // Parse and as a print the resulting mindmap (this is temporary)
  // TODO: Error handling
  yyparse();
  fclose(yyin);
  mm_print(mmap);  
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
  return true;
}

extern "C" bool to_dot(const char *mmfilepath, const char *dotfilepath) {
  parse(mmfilepath);

  std::ofstream ofs{dotfilepath};
  // Generate the output
  // write the header
  ofs << "digraph G {\n";
  ofs << "bgcolor=\"#ffffff\"\n";
  // Layout from left to right
  ofs << "rankdir=LR;\n";
  
  // Write out nodes
  node_to_dot(ofs, mm_get_root(mmap));
  
  // Write footer
  ofs << "}\n";

  // Free mindmap
  mm_delete(mmap);
  return true;
}
} // namespace mm
