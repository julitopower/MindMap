#ifndef MM_HPP
#define MM_HPP

/*! \brief Handler to a mindmap */
typedef void *MM_HDL;

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

/*! \brief Allocate a new mindmap.
 *
 * The memory allocated must be released with mm_delete
 */
MM_HDL mm_new();

/*! \brief Get the root node of a mindmap */
Node *mm_get_root(MM_HDL mm);
/*! \brief Release the memory allocated for a mindmap */
void mm_delete(MM_HDL mm);

// Mindmap takes ownership of the content memory
void mm_add_node(MM_HDL mm, int level, char *content);

// Not implemented yet
void mm_add_node_attribute(MM_HDL mm, struct Attribute *attribute);
// Not implemented yet
void mm_add_edge_attribute(MM_HDL mm, struct Attribute *attribute);
/*! \brief Print a mindmapt to stdout */
void mm_print(MM_HDL mm);
#endif
