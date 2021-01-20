#include <cstdio>
#include <fstream>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <graphviz/gvcext.h>
#include <iostream>
#include <memory>
#include <string>

#include "mindmap/mm.hpp"
#include "mindmap/utils.hpp"
// Mindmap C data structures
#include "cmm_int.h"

// We use the C interface of flex/bison, and also defined the
// mindmap datastructures in C
extern "C" {
// Flex/Bison
#include "parser.h"

extern FILE *yyin;
extern void *mmap;
}

namespace {

/* Add surounding quotes to a string. Allocates new memory, and does
   not release the memory provided. It always returns the new allocated
   memory */
char *quote(char *str) {
  char *out = static_cast<char *>(calloc(strlen(str) + 3, sizeof(char)));
  if (str[0] != '\"') {
    strcat(out, "\"");
    strcat(out, str);
    strcat(out, "\"");
    return out;
  }
  strcpy(out, str);
  return out;
}

/* Given a node, it recursively prints the entire tree
   as a Graphviz digraph, excluding header and footer */
void node_to_dot(std::ofstream &os, Node *node, bool master = false) {
  if (node->nchildren > 0) {
    char *content = quote(node->content);
    if (master) {
      os << content << " [peripheries=2, fontname=\"alial bold\"];\n";
    }
    for (auto i = 0; i < node->nchildren; ++i) {
      char *child_content = quote(node->children[i]->content);

      os << content << " -> " << child_content;

      if (master) {
        os << "[color=red]";
      }

      os << std::endl;

      free(child_content);
      node_to_dot(os, node->children[i]);
    }
    free(content);
  }
}

void parse(const char *mmfilepath) {
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

  gvFreeContext(gvc);
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

  // global node attributes
  ofs << "graph [bgcolor=\"#ffcc40\", splines=curved, root=\"JustDoIt\", "
         "layout=dot, K=1.1,  overlap=false, start=42, maxiter=100000]\n";
  ofs << "node [shape=box, "
         "color=\"#2e6e3f\"fillcolor=\"#feffed\"style=\"filled,rounded\", "
         "fontname=arial, fontsize=11]\n";
  ofs << "edge [arrowhead = open, color=\"#002b36\"]\n";

  // Write out nodes
  node_to_dot(ofs, mm_get_root(mmap), true);

  // Write footer
  ofs << "}\n";

  // Free mindmap
  mm_delete(mmap);
  return true;
}
} // namespace mm
