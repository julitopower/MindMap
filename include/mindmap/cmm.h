#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Handler to a mindmap */
typedef void *MM_HDL;

// Mindmap takes ownership of the content memory
void mm_add_node(MM_HDL mm, int level, char *content);

#ifdef __cplusplus
}
#endif
