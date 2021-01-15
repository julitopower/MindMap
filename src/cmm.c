#include "mindmap/cmm.h"
#include <stdio.h>
#include <stdlib.h>

Node *node_new(int level, char *content) {
  // Allocate and initialize a Node
  Node *node = malloc(sizeof(Node));
  node->level = level;
  node->content = content;
  node->node_attributes = NULL;
  node->edge_attributes = NULL;
  node->children = NULL;
  node->nchildren = 0;
  return node;
}

void node_print_rec(Node *node) {
  for (int i = 0; i < node->level; ++i) {
    printf("*");
  }

  printf(" L(%d) [%s]\n", node->level, node->content);

  if (node->children != NULL) {
    for (int i = 0; i < node->nchildren; ++i) {
      node_print_rec(node->children[i]);
    }
  }
}

void node_add_child(Node *parent, Node *child) {
  if (parent->children == NULL) {
    parent->children = malloc(sizeof(Node **));
  } else {
    parent->children =
        realloc(parent->children, sizeof(Node **) * (parent->nchildren + 1));
  }
  parent->children[parent->nchildren++] = child;
}

void node_free(Node *node) {
  if (node->children != NULL) {
    for (int i = 0; i < node->nchildren; ++i) {
      node_free(node->children[i]);
    }
    free(node->children);
  }

  free(node->content);
  free(node);
}

/*
 * A Mindmap is a handler into the mindmap library. It contains a pointer to the
 * root node of the mindmap, a pointer to the last node added, and a stack of *
 * Nodes used to represent active scopes:
 *
 * As we read the graph text file, each node we encounter opens a new scope.
 * While a scope is open, children nodes can be added to it. Whenever we read a
 * new node, we look at its level, which must be in the range [1, current + 1].
 There are two cases:

 * (a) If the level is current + 1, the new node is child of the current, so we
 * add it to the children list, and push it to the stack, because a new scope
 * has been inserted.
 *
 * (b) If the level is =< than the current node, then we have to
 * close some of the scopes in the stack (or find the correct parent). So we pop
 * values from the stack until we find a Node whose level + 1 is == to the level
 * of the node we are processing.
 */
typedef struct Mindmap Mindmap;
struct Mindmap {
  Node *root;
  Node *current;
  Node **scopes;
  int nscopes;
  int scopes_capacity;
};

MM_HDL mm_new() {
  // Allocate space for the mindmap
  Mindmap *mm = (Mindmap *)malloc(sizeof(Mindmap));
  mm->root = NULL;
  mm->current = mm->root;

  // Initialize the scopes stack with 10 slots
  mm->scopes = (Node **)malloc(sizeof(Node *) * 10);
  mm->nscopes = 0;
  mm->scopes_capacity = 10;
  return mm;
}

void mm_delete(MM_HDL mm) {
  Mindmap *mm_hdl = (Mindmap *)mm;

  // Free the scopes memory, which is simply an array
  free(mm_hdl->scopes);
  // Free all the nodes, starting from the root
  node_free(mm_hdl->root);
  // Free the actual mindmap memory
  free(mm_hdl);
}

Node* mm_get_root(MM_HDL mm) {
  Mindmap *mm_hdl = (Mindmap *)mm;
  return mm_hdl->root;
}

void mm_print(MM_HDL mm) { node_print_rec(((Mindmap *)mm)->root); }

void mm_add_node(MM_HDL mm, int level, char *content) {
  Mindmap *mm_hdl = (Mindmap *)mm;

  // Allocate and initialize a Node
  Node *node = node_new(level, content);

  // If this is the first node
  if (mm_hdl->root == NULL) {
    mm_hdl->root = node;
    mm_hdl->current = node;
  }

  // Manage the scopes.
  // Do we have enough memory? TODO: This check is too early, and actually fails
  // in situations it shouldn't
  if (mm_hdl->scopes_capacity == mm_hdl->nscopes) {
    fprintf(stderr, "Scopes stack is full. It contains %d elelements\n",
            mm_hdl->nscopes);
    node_free(node);
    mm_delete(mm);
    exit(1);
  }

  // If there is an active scope
  if (mm_hdl->nscopes > 0) {
    // This is equivalent to a stack.peek()
    Node *active_node = mm_hdl->scopes[mm_hdl->nscopes - 1];
    // node is a child of the previous node
    if (node->level == active_node->level + 1) {
      // Add child to parent
      node_add_child(active_node, node);
    } else if (node->level > active_node->level +
                                 1) { // TODO: Can we check this as a semantic
                                      // action in the grammar
      fprintf(stderr, "Incorrect child level %d, for parent of level %d",
              node->level, active_node->level);
      node_free(node);
      mm_delete(mm);
      exit(1);
    } else {
      // Pop one scope
      --mm_hdl->nscopes;

      while (mm_hdl->nscopes > 0 &&
             (active_node = mm_hdl->scopes[mm_hdl->nscopes - 1], // Peek
              active_node->level != node->level - 1)) {          // Check
        --mm_hdl->nscopes;
      }

      if (mm_hdl->nscopes == 0) {
        fprintf(stderr, "Incorrect structure, no parent found\n");
        node_free(node);
        mm_delete(mm);
        exit(1);
      }

      node_add_child(active_node, node);
    }
  }

  mm_hdl->scopes[mm_hdl->nscopes++] = node;
}
void mm_add_node_attribute(MM_HDL mm, struct Attribute *attribute);
void mm_add_edge_attribute(MM_HDL mm, struct Attribute *attribute);
