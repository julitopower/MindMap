A mindmap is a directe graph with a single root node. Each node support configurable attributes. Edges also support configurable attributes, but for simplicity all edges with the same source node share the same attributes.

# Design tenets

* Simple is better. This format is intended for humans, and so we value simplicity above other things
* Close to existing formats.
* Simple to parse.
* Avoid repetition. If a user wants to apply the same attributes to multiple elements, it should not have to repeat the attribute definitions on and on.

# Syntactic Elements

LEVEL: "*"+
NAME: String - {"="}
ARROW: "->"
NL: "\n"

Mindmap: Version (Comment | Node)+
Comment: # String NL
Version: NUMBER.NUMBER NL
Node: LEVEL NAME [AttributesList] [ARROW AttributesList] NL
AttributesList: "[" (EdgeAttributes | NodeAttributes) "]"
EdgeAttributes: EdgeAttribute {"," EdgeAttribute)}
NodeAttributes: NodeAttribute {"," NodeAttribute}
EdgeAttribute: EdgeAttributeName "=" Value 
NodeAttribute: ModeAttributeName "=" Value
EdgeAttributeName: "line" | "color" | "width" | "arrow"
NodeAttributeName: "line" | "color"


# Further format restrictions

Given a parent node all descendants children must appear after it, and before any other node at the same level or above it. We can consider that the definition of a node opens a scope. Elements can only be added to the scope while it is open. As soon as a node at the same or higher level is defined, the current scope it is closed. This idea seems a bit difficult to model as a formal grammar.

```markdown
0.1
* Parent[line=solid,width=10] -> [line=dashed]
** Child-1
** Child-2
** Child-3
*** G-child-1
* Sibling
```

# Recursive descent design

* Each rule becomes a function
* Lexer rules just pattern match

