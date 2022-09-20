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

#include <anyvalueeditor/abstract_anyvalue_buildnode.h>
#include <mvvm/utils/container_utils.h>

#include <stdexcept>

namespace anyvalueeditor
{

AbstractAnyValueBuildNode::AbstractAnyValueBuildNode(sup::dto::AnyValue &&value)
    : m_value(std::move(value))
{
}

void AbstractAnyValueBuildNode::Consume(sup::dto::AnyValue &&value)
{
  m_value = std::move(value);
}

sup::dto::AnyValue AbstractAnyValueBuildNode::MoveAnyValue() const
{
  return std::move(m_value);
}

//! Returns true if it is possible to add value node. This will be the case in one of three cases:
//! 1) the stack is empty 2) last node denotes that we are about to add a field to the struct
//! 3) last node denote that we are about to add an element to the array.

bool AbstractAnyValueBuildNode::CanAddValueNode(const std::stack<node_t> &stack)
{
  static const std::vector<NodeType> expected_types{NodeType::kStartArrayElement,
                                                    NodeType::kStartField};

  return stack.empty() || mvvm::utils::Contains(expected_types, stack.top()->GetNodeType());
}

std::string AbstractAnyValueBuildNode::GetFieldName() const
{
  return m_field_name;
}

void AbstractAnyValueBuildNode::SetFieldName(const std::string &name)
{
  m_field_name = name;
}

void AbstractAnyValueBuildNode::AddMember(const std::string &name, const sup::dto::AnyValue &value)
{
  throw std::runtime_error("Error in AbstractAnyValueBuildNode::AddMember() : not implemented");
}

void AbstractAnyValueBuildNode::AddElement(const sup::dto::AnyValue &value)
{
  throw std::runtime_error("Error in AbstractAnyValueBuildNode::AddMember() : not implemented");
}

}  // namespace anyvalueeditor
