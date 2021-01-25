#include "mindmap/mm.hpp"
#include "mindmap/cmm.h"
#include <sstream>
#include <stdexcept>

int mm_add_node(MM_HDL mm, int level, char *content) {
  mm::MindMapBuilder *mm_builder = static_cast<mm::MindMapBuilder *>(mm);
  try {
    mm_builder->add_node(level, content);
  } catch (const std::invalid_argument &ex) {
    return 1;
  }
  return 0;
}

const char *mm_last_error_msg(MM_HDL mm) {
  mm::MindMapBuilder *mm_builder = static_cast<mm::MindMapBuilder *>(mm);
  return mm_builder->last_error_msg().c_str();
}

namespace mm {

////////////////////////////////////////////////////////////////////////////////
// mm::Node methods
////////////////////////////////////////////////////////////////////////////////

Node::Node(std::size_t level, const std::string &content)
    : level_{level}, content_{content} {}

Node::Node(std::unique_ptr<Node> &&ptr)
    : level_{ptr->level()}, content_{std::move(ptr.get()->content_)},
      attributes_{std::move(ptr.get()->attributes_)},
      children_{std::move(ptr.get()->children_)} {}

Node::Node(Node &&node)
    : level_(node.level_), content_{std::move(node.content_)},
      attributes_{std::move(node.attributes_)}, children_{std::move(
                                                    node.children_)} {}

void Node::set_attribute(const std::string &key, const std::string &value) {
  attributes_[key] = value;
}

void Node::add_child(std::unique_ptr<Node> &&child_ptr) {
  children_.push_back(std::move(child_ptr));
}

const decltype(Node::children_) &Node::children() const { return children_; }

const decltype(Node::attributes_) &Node::attributes() const {
  return attributes_;
}

const std::string &Node::content() const { return content_; }

std::size_t Node::level() const { return level_; }

////////////////////////////////////////////////////////////////////////////////
// End of mm::Node methods
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// mm::MindMap methods
////////////////////////////////////////////////////////////////////////////////
MindMap::MindMap(std::unique_ptr<Node> &&root) : root_{std::move(root)} {}

MindMap::MindMap(MindMap &&mindmap) : root_{std::move(mindmap.root_)} {}

const Node &MindMap::root() const { return *root_.get(); }
////////////////////////////////////////////////////////////////////////////////
// End of mm::MindMap methods
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// mm::MindMapBuilder methods
////////////////////////////////////////////////////////////////////////////////
MindMapBuilder::MindMapBuilder() {}

MindMap MindMapBuilder::build() { return MindMap(std::move(root_)); }

const std::string &MindMapBuilder::last_error_msg() const {
  return last_error_msg_;
}

MindMapBuilder &MindMapBuilder::add_node(std::size_t level,
                                         const std::string &content) {
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
      std::stringstream ss;
      ss << "Incorrect child level " << node->level() << " for parent of level "
         << active_node->level() << "\n";
      throw std::invalid_argument{ss.str()};
    } else {
      // Keep poping scopes until we find the right parent
      scopes_.pop_back();
      while (!scopes_.empty() && (active_node = scopes_.back(),
                                  active_node->level() + 1 != node->level())) {
        scopes_.pop_back();
      }

      if (scopes_.empty()) {
        // Error, we didn't find a parent
        // Note that the new node is managed by node_ptr and will get
        // released correctly.
        throw std::invalid_argument{"Incorrect structure, no parent found\n"};
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
////////////////////////////////////////////////////////////////////////////////
// End of mm::MindMapBuilder methods
////////////////////////////////////////////////////////////////////////////////

} // namespace mm
