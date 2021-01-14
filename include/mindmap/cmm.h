#ifndef MM_HPP
#define MM_HPP

typedef void *MM_HDL;

struct Attribute;
struct Node;

MM_HDL mm_new();
void mm_delete(MM_HDL mm);

// Mindmap takes ownership of the content memory
void mm_add_node(MM_HDL mm, int level, char *content);

void mm_add_node_attribute(MM_HDL mm, struct Attribute *attribute);
void mm_add_edge_attribute(MM_HDL mm, struct Attribute *attribute);
void mm_print(MM_HDL mm);
#endif
