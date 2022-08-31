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

struct DomainAnyValueBuilder::DomainAnyValueBuilderImpl
{
  const AnyValueItem &m_item;
  AnyValueBuildAdapter m_build_adapter;
  std::stack<const AnyValueItem *> m_stack;

  explicit DomainAnyValueBuilderImpl(const AnyValueItem &item) : m_item(item)
  {
    m_stack.push(&item);
    ProcessItemStack();
  }

  void ProcessItemStack()
  {
    while (!m_stack.empty())
    {
      const auto item = m_stack.top();
      m_stack.pop();
      ProcessItem(item);
    }
  }

  void ProcessItem(const AnyValueItem *item)
  {
    if (m_item.IsScalar())
    {
      // empty member name denotes top level scalar
      m_build_adapter.AddMember("", GetAnyValueFromScalar(*item));
    }
  }
};

DomainAnyValueBuilder::DomainAnyValueBuilder(const AnyValueItem &item)
    : p_impl(std::make_unique<DomainAnyValueBuilderImpl>(item))
{
}

DomainAnyValueBuilder::~DomainAnyValueBuilder() = default;

sup::dto::AnyValue DomainAnyValueBuilder::GetAnyValue() const
{
  return p_impl->m_build_adapter.MoveAnyValue();
}

}  // namespace anyvalueeditor
