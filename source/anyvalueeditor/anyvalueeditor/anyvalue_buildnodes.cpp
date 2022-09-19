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

StartStructBuildNode::StartStructBuildNode(const std::string &struct_name)
    : AbstractAnyValueBuildNode(::sup::dto::EmptyStruct(struct_name))

{
}

AbstractAnyValueBuildNode::NodeType StartStructBuildNode::GetNodeType() const
{
  return NodeType::kStartStruct;
}

// ----------------------------------------------------------------------------
// StartStructBuildNode
// ----------------------------------------------------------------------------

bool StartStructBuildNode::Process(std::stack<node_t> &stack)
{
  if (CanAddValueNode(stack))
  {
    return true;
  }

  throw std::runtime_error("Error in AnyValueBuildNode::Process(): AnyValueNode can not be added");
}

bool StartStructBuildNode::IsStartStructNode() const
{
  return true;
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
  // do nothing, do not add itself to the stack
  return false;
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

  if (stack.empty() || !stack.top()->IsStartStructNode())
  {
    throw std::runtime_error(
        "Error in StartFieldBuildNode::Process(): last node is not a structure");
  }

  return true;
}

bool StartFieldBuildNode::IsStartFieldNode() const
{
  return true;
}

// ----------------------------------------------------------------------------
// EndFieldBuildNode
// ----------------------------------------------------------------------------

AbstractAnyValueBuildNode::NodeType EndFieldBuildNode::GetNodeType() const
{
  return NodeType::kEndField;
}

bool EndFieldBuildNode::Process(std::stack<node_t> &stack)
{
  static const std::vector<NodeType> expected_types{NodeType::kValue, NodeType::kEndStruct,
                                                    NodeType::kEndArray};
  if (stack.empty() || mvvm::utils::Contains(expected_types, stack.top()->GetNodeType()))
  {
    throw std::runtime_error("Error in EndFieldBuildNode::Process(): wrong node type");
  }

  auto value = stack.top()->MoveAnyValue();
  stack.pop();

  if (stack.empty() || stack.top()->GetNodeType() != NodeType::kStartField)
  {
    throw std::runtime_error("Error in EndFieldBuildNode::Process(): wrong node type");
  }

  auto field_name = stack.top()->GetFieldName();
  stack.pop();

  if (stack.empty())
  {
    throw std::runtime_error("Error in EndFieldBuildNode::Process(): wrong node type");
  }

  stack.top()->AddMember(field_name, value);

  // we don't need to save the node in the stack, all job is already done
  return false;
}

}  // namespace anyvalueeditor
