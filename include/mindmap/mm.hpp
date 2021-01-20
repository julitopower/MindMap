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
  Node(std::size_t level, const std::string &content)
      : level_{level}, content_{content} {}

  /*! \brief Steal node from unique pointer */
  explicit Node(std::unique_ptr<Node> &&ptr) :
      level_{ptr->level()},
      content_{std::move(ptr.get()->content_)},
      attributes_{std::move(ptr.get()->attributes_)},
      children_{std::move(ptr.get()->children_)} {}

  /*! \brief Move copy constructor */
  Node(Node &&node)
      : level_(node.level_), content_{std::move(node.content_)},
        attributes_{std::move(node.attributes_)},
        children_{std::move(node.children_)} {}  

  /*! \brief Add or overwrite an attribute */
  void set_attribute(const std::string &key, const std::string &value) {
    attributes_[key] = value;
  }

  void add_child(std::unique_ptr<Node>&& child_ptr) {
    children_.push_back(std::move(child_ptr));
  }  

  /*! \brief Getter for children */
  const decltype(children_)& children() const {
    return children_;
  }

  /*! \brief Getter for attributes */
  const decltype(attributes_)& attributes() const {
    return attributes_;
  }  

  /*! \brief Getter for content */
  const std::string& content() const { return content_; }

  /*! \brief Getter for content */
  std::size_t level() const { return level_; }  
  
};

class MindMap {
private:
  std::unique_ptr<Node> root_;

 public:
  MindMap(std::unique_ptr<Node>&& root) : root_{std::move(root)} {}
  MindMap(MindMap&& mindmap) : root_{std::move(mindmap.root_)} {}
  const Node& root() const { return *root_.get(); }
};

class MindMapBuilder {
 private:
  std::unique_ptr<Node> root_;
  std::vector<Node*> scopes_;  
 public:
  MindMapBuilder() {}
  MindMapBuilder(const MindMapBuilder&) = delete;
  MindMapBuilder(MindMapBuilder&&) = delete;

  MindMap build() {
    return MindMap(std::move(root_));
  }
  
  MindMapBuilder& add_node(std::size_t level, const std::string& content) {
    // This unique_ptr ensures that this memory gets released in the case
    // of a failure
    auto node_ptr = std::make_unique<Node>(level, content);
    auto node = node_ptr.get();

    // Is this the first node?
    if (!root_) {
      root_.reset(node);
    }

    // Is there an active scope?
    if (!scopes_.empty()) {
      // YES
      auto active_node = scopes_.back();
      if (node->level() == active_node->level() + 1) {
        active_node->add_child(std::move(node_ptr));
      } else if (node->level() > active_node->level() + 1) {
        // Error, level of new node is too large
        // Note that the new node is managed by node_ptr and will get
        // released correctly.
        std::cerr << "Incorrect child level " << node->level()
                  << " for parent of level " << active_node->level() << "\n";
      } else {
        // Keep poping scopes until we find the right parent
        scopes_.pop_back();
        while (!scopes_.empty() &&
               (active_node = scopes_.back(), active_node->level() + 1 != node->level())) {
          scopes_.pop_back();
        }

        if (scopes_.empty()) {
          // Error, we didn't find a parent
          // Note that the new node is managed by node_ptr and will get
          // released correctly.
          std::cerr << "Incorrect structure, no parent found\n";
        }

        active_node->add_child(std::move(node_ptr));
      }
    } 

    // The node we just inserted creates a new scope
    scopes_.push_back(node);

    // Make sure to release the smart pointer, so that node
    // memory is not deleted
    node_ptr.release();    

    return *this;    
  }
};
} // namespace mm
