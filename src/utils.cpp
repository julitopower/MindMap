#include <cstdio>
#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <graphviz/gvcext.h>

#include "mindmap/utils.hpp"

namespace mm {
extern "C" bool to_png(const char *dotfilepath, const char *outputfilepath) {
  // Get a context
  GVC_t *gvc = gvContext();
  // Read a file into a graph
  FILE *fin = fopen(dotfilepath, "r");
  Agraph_t *graph = agread(fin, 0);

  gvLayout(gvc, graph, "dot");
  gvRenderFilename(gvc, graph, "png", outputfilepath);
  gvFreeLayout(gvc, graph);
  agclose(graph);
  fclose(fin);
  return true;
}
} // namespace mm
