# Introduction

This document describes the software design necessary to support flexible mindmap attributes. Almost all attributes are concerned with how the mindmap is displayed.

The actual syntax for attributes and other necessary elements is described elsewhere.

# Requirements

* Graph, node and edge attributes
* Global attributes
* Global attributes can be overridden by element
* Attributes applied depending on a predicate
  * Node level
  * Edge origin level
  * Node Context length
* Support "themes" or "Styles"

# Attributes

Attributes apply to the mindmap, nodes, and edges. At a high level we have the following types of attributes:

* Global attributes
  * Conditional
  * Unconditional
* Local attributes
  * Unconditional
  
# Attribute application

We can consider all attributes conditional. Each attribute is conditional on a set of predicates, which always include at least the type of element they apply to. All predicates must evaluate to true in order to apply an attribute to an element.

To calculate the attributes of an element we:

* Filter global attributes for the element
* Add/override with local attributes

This provides a single mechanism for the handling of all attributes.

# Software design

## C implementation with scopes

Use scopes and use the first value found while going from the closes to the farthest scope.

```C
typedef struct Scope Scope;
struct Scope {
    Struct* parent;
    AttributesBag* attributes;
};

Scope* global = read_global_scope(filepath);
Scope local = {.parent = global, .attributes = node->attributes};
```

Now, with this approach we would do the following:

```C
// Traverse all global attributes, but pick up local overrides if
// available
for_each attribute in global {
    if get_attribute(local, attribute.name).eval(node) {
        emit(os, attribute, writer);
        remove_attribute(local attribute.name);
    }
}

// Now evaluate apply the rest of the local attributes.
for_each attribute in local {
    if get_attribute(local, attribute.name).eval(node) {
        emit(os, attribute, writer);
        remove_attribute(local attribute.name);
    }
}
```

This work with two levels of scopes, but doesn't generalize to N, and it feels really convoluted. A much simpler approach would:

* Create a new attribute bag
* Traverse global scope, filter by element type and add to the bag
* Traverse local scope, filter by element type and add/override to the bag

This easily generalizes to N scopes, since after processing one scope, one just have to go the the child scope and repeat the process. **I like this approach. It requires that the type predicate becomes a different concept from the other predicates though. So, in a way, it is not as general.**
