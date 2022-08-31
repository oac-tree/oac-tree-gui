/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "anyvalueeditor/domain_anyvalue_builder.h"

#include <anyvalueeditor/anyvalue_build_adapter.h>
#include <anyvalueeditor/anyvalue_item.h>
#include <anyvalueeditor/scalar_conversion_utils.h>
#include <sup/dto/anyvalue.h>

#include <stack>

namespace anyvalueeditor
{

struct Node
{
  const AnyValueItem* m_item{nullptr};  //!< Current item in the hierarchy.
  std::string m_name;                   //!< The name under which the value is known to its parent.
  bool m_is_visited{false};  //!< True if `value` is a struct and all children are processed.

  Node(const AnyValueItem* item, const std::string& name) : m_item(item), m_name(name) {}
};

struct DomainAnyValueBuilder::DomainAnyValueBuilderImpl
{
  AnyValueBuildAdapter m_builder;
  std::stack<Node> m_stack;

  explicit DomainAnyValueBuilderImpl(const AnyValueItem& item)
  {
    m_stack.push({&item, std::string()});
    ProcessItemStack();
  }

  void ProcessItemStack()
  {
    while (!m_stack.empty())
    {
      auto& node = m_stack.top();

      if (node.m_item->IsScalar())
      {
        ProcessScalarItem(node);
      }
      else
      {
        m_stack.pop();
      }
    }
  }

  void ProcessScalarItem(Node& node)
  {
    // It's a scalar field. Let's add corresponding field to the AnyValue and remove node from
    // stack. We don't need it anymore.
    m_builder.AddMember(node.m_name, GetAnyValueFromScalar(*node.m_item));
    m_stack.pop();
  }
};

DomainAnyValueBuilder::DomainAnyValueBuilder(const AnyValueItem& item)
    : p_impl(std::make_unique<DomainAnyValueBuilderImpl>(item))
{
}

DomainAnyValueBuilder::~DomainAnyValueBuilder() = default;

sup::dto::AnyValue DomainAnyValueBuilder::GetAnyValue() const
{
  return p_impl->m_builder.MoveAnyValue();
}

}  // namespace anyvalueeditor
