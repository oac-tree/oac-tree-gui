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
  return false;
}

}  // namespace anyvalueeditor
