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

// ----------------------------------------------------------------------------
// EndStructBuildNode
// ----------------------------------------------------------------------------

bool EndStructBuildNode::Process(std::stack<node_t> &stack)
{
  // do nothing, do not add itself to the stack
  return false;
}

// ----------------------------------------------------------------------------
// StartFieldBuildNode
// ----------------------------------------------------------------------------

StartFieldBuildNode::StartFieldBuildNode(const std::string &field_name) : m_field_name(field_name)
{
}

bool StartFieldBuildNode::Process(std::stack<node_t> &stack)
{
  return false;
}

bool StartFieldBuildNode::IsStartFieldNode() const
{
  return true;
}

// ----------------------------------------------------------------------------
// EndFieldBuildNode
// ----------------------------------------------------------------------------

bool EndFieldBuildNode::Process(std::stack<node_t> &stack)
{
  return false;
}

}  // namespace anyvalueeditor
