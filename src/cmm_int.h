#pragma once

#include "mindmap/cmm.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief An attribute of a graph, node or edge
 * An attribute represents a <key, value>, with both key and value being
 * strings.
 */
typedef struct Attribute Attribute;
struct Attribute {
  char *key;
  char *value;
};

/*! \brief A node in a mindmap
 * A node in a mindmap has some depth, content Attributes for how to draw it,
 * and attribute for how to draw the edges between itsel and its children.
 */
typedef struct Node Node;
struct Node {
  int level;
  char *content;
  struct Attribute *node_attributes;
  struct Attribute *edge_attributes;
  Node **children;
  int nchildren;
};

  #ifdef __cplusplus
}
#endif  
