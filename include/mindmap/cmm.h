#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Handler to a mindmap */
typedef void *MM_HDL;

// Mindmap takes ownership of the content memory
// 0 -> success
// 1 -> failures
int mm_add_node(MM_HDL mm, int level, char *content);

// Get the message of the last error. Memory must not be
// released by the caller
const char *mm_last_error_msg(MM_HDL mm);

#ifdef __cplusplus
}
#endif
