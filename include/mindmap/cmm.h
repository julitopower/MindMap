#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Handler to a mindmap */
typedef void *MM_HDL;

typedef struct Node Node;
typedef struct Attribute Attribute;

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

#ifdef __cplusplus
}
#endif
