/******************************************************************************
 *
 * Project       : Supervision and automation system EPICS interface
 *
 * Description   : Library of SUP components for EPICS network protocol
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalueeditor/anyvalue_buildnodes.h"

#include <mvvm/utils/container_utils.h>

#include <iostream>
#include <stdexcept>

namespace anyvalueeditor
{

// ----------------------------------------------------------------------------
// AnyValueBuildNode
// ----------------------------------------------------------------------------

AnyValueBuildNode::AnyValueBuildNode(sup::dto::AnyValue &&value)
    : AbstractAnyValueBuildNode(std::move(value))
{
}

AbstractAnyValueBuildNode::NodeType AnyValueBuildNode::GetNodeType() const
{
  return NodeType::kValue;
}

bool AnyValueBuildNode::Process(std::stack<node_t> &stack)
{
  if (CanAddValueNode(stack))
  {
    return true;
  }

  throw std::runtime_error("Error in AnyValueBuildNode::Process(): AnyValueNode can not be added");
}

// ----------------------------------------------------------------------------
// StartStructBuildNode
// ----------------------------------------------------------------------------

StartStructBuildNode::StartStructBuildNode(const std::string &struct_name)
    : AbstractAnyValueBuildNode(::sup::dto::EmptyStruct(struct_name))

{
}

AbstractAnyValueBuildNode::NodeType StartStructBuildNode::GetNodeType() const
{
  return NodeType::kStartStruct;
}

bool StartStructBuildNode::Process(std::stack<node_t> &stack)
{
  if (CanAddValueNode(stack))
  {
    return true;
  }

  throw std::runtime_error("Error in AnyValueBuildNode::Process(): AnyValueNode can not be added");
}

void StartStructBuildNode::AddMember(const std::string &name, const sup::dto::AnyValue &value)
{
  m_value.AddMember(name, value);
}

// ----------------------------------------------------------------------------
// EndStructBuildNode
// ----------------------------------------------------------------------------

AbstractAnyValueBuildNode::NodeType EndStructBuildNode::GetNodeType() const
{
  return NodeType::kEndStruct;
}

bool EndStructBuildNode::Process(std::stack<node_t> &stack)
{
  // expecting StartStruct node at the top
  if (stack.empty() || stack.top()->GetNodeType() != NodeType::kStartStruct)
  {
    throw std::runtime_error("Error in EndFieldBuildNode::Process(): wrong node type");
  }

  Consume(stack.top()->MoveAnyValue());
  stack.pop();

  return true;
}

// ----------------------------------------------------------------------------
// StartFieldBuildNode
// ----------------------------------------------------------------------------

StartFieldBuildNode::StartFieldBuildNode(const std::string &field_name)
    : AbstractAnyValueBuildNode()
{
  SetFieldName(field_name);
}

AbstractAnyValueBuildNode::NodeType StartFieldBuildNode::GetNodeType() const
{
  return NodeType::kStartField;
}

bool StartFieldBuildNode::Process(std::stack<node_t> &stack)
{
  if (GetFieldName().empty())
  {
    throw std::runtime_error("Error in StartFieldBuildNode::Process(): fieldname is not defined");
  }

  if (stack.empty() || stack.top()->GetNodeType() != NodeType::kStartStruct)
  {
    throw std::runtime_error(
        "Error in StartFieldBuildNode::Process(): last node is not a structure");
  }

  return true;
}

// ----------------------------------------------------------------------------
// EndFieldBuildNode
// ----------------------------------------------------------------------------

AbstractAnyValueBuildNode::NodeType EndFieldBuildNode::GetNodeType() const
{
  return NodeType::kEndField;
}

//! Processes stack by removing 2 last build nodes (the node with the value, and
//! StartFieldBuildeNode)
bool EndFieldBuildNode::Process(std::stack<node_t> &stack)
{
  static const std::vector<NodeType> expected_types{NodeType::kValue, NodeType::kEndStruct,
                                                    NodeType::kEndArray};

  if (stack.empty() || !mvvm::utils::Contains(expected_types, stack.top()->GetNodeType()))
  {
    throw std::runtime_error("Error in EndFieldBuildNode::Process(): wrong node type");
  }

  // removing value node (scalar, struct or array), keeping the value for later reuse
  auto value = stack.top()->MoveAnyValue();
  stack.pop();

  if (stack.empty() || stack.top()->GetNodeType() != NodeType::kStartField)
  {
    throw std::runtime_error("Error in EndFieldBuildNode::Process(): wrong node type");
  }

  // removing StartFieldNode, keeping the name for later reuse
  auto field_name = stack.top()->GetFieldName();
  stack.pop();

  if (stack.empty())
  {
    throw std::runtime_error("Error in EndFieldBuildNode::Process(): wrong node type");
  }

  // We expecting to find here StartStructBuildNode, adding a new member to it.
  stack.top()->AddMember(field_name, value);

  // we don't need to save the node in the stack, all job is already done
  return false;
}

// ----------------------------------------------------------------------------
// StartArrayBuildNode
// ----------------------------------------------------------------------------

StartArrayBuildNode::StartArrayBuildNode(const std::string &array_name) {}

AbstractAnyValueBuildNode::NodeType StartArrayBuildNode::GetNodeType() const
{
  return NodeType::kStartArray;
}

bool StartArrayBuildNode::Process(std::stack<node_t> &stack)
{
  return true;
}

// ----------------------------------------------------------------------------
// EndArrayBuildNode
// ----------------------------------------------------------------------------

AbstractAnyValueBuildNode::NodeType EndArrayBuildNode::GetNodeType() const
{
  return NodeType::kEndArray;
}

bool EndArrayBuildNode::Process(std::stack<node_t> &stack)
{
  return true;
}

// ----------------------------------------------------------------------------
// StartArrayElementBuildNode
// ----------------------------------------------------------------------------

AbstractAnyValueBuildNode::NodeType StartArrayElementBuildNode::GetNodeType() const
{
  return NodeType::kStartArrayElement;
}

bool StartArrayElementBuildNode::Process(std::stack<node_t> &stack)
{
  return true;
}

// ----------------------------------------------------------------------------
// StartArrayElementBuildNode
// ----------------------------------------------------------------------------

AbstractAnyValueBuildNode::NodeType EndArrayElementBuildNode::GetNodeType() const
{
  return NodeType::kEndArrayElement;
}

bool EndArrayElementBuildNode::Process(std::stack<node_t> &stack)
{
  return false;
}

}  // namespace anyvalueeditor
