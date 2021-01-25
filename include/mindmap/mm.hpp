#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>


namespace mm {
/*

  The way we envisioned this being used is as follows:

  * Client creates a MindMapBuilder class
  * Client invokes n time builder.add_node(...)
  * Client invokes once builder.build()

  After calling buider.build() once, the builder gets reset, and calling
  build() again would just produce an empty MindMap.

  MindMap can be treated as a value object. It implements the necessary
  move constructors.
 */

/*! \brief A node in a mindmap
 * A node in a mindmap has some depth, content Attributes for how to draw it,
 * and attribute for how to draw the edges between itsel and its children.
 */
class Node {
private:
  /* The depth in the tree*/
  std::size_t level_;
  /* The text to display */
  std::string content_;
  /* Attributes for display */
  std::map<std::string, std::string> attributes_;
  /* The childen of this node */
  std::vector<std::unique_ptr<Node>> children_;

public:
  /*! \brief Create a node without children */
  Node(std::size_t level, const std::string &content);

  /*! \brief Steal node from unique pointer */
  explicit Node(std::unique_ptr<Node> &&ptr);

  /*! \brief Move copy constructor */
  Node(Node &&node);

  /*! \brief Add or overwrite an attribute */
  void set_attribute(const std::string &key, const std::string &value);

  void add_child(std::unique_ptr<Node>&& child_ptr);

  /*! \brief Getter for children */
  const decltype(children_)& children() const;

  /*! \brief Getter for attributes */
  const decltype(attributes_)& attributes() const;

  /*! \brief Getter for content */
  const std::string& content() const;

  /*! \brief Getter for content */
  std::size_t level() const;
  
};

class MindMap {
private:
  std::unique_ptr<Node> root_;

 public:
  MindMap(std::unique_ptr<Node>&& root);
  MindMap(MindMap&& mindmap);
  const Node& root() const;
};

class MindMapBuilder {
 private:
  std::unique_ptr<Node> root_;
  std::vector<Node*> scopes_;
  std::string last_error_msg_;
 public:
  MindMapBuilder();
  MindMapBuilder(const MindMapBuilder&) = delete;
  MindMapBuilder(MindMapBuilder&&) = delete;

  MindMap build();
  
  MindMapBuilder& add_node(std::size_t level, const std::string& content);
  const std::string& last_error_msg() const;
};
} // namespace mm
