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
#include <sup/dto/anyvalue.h>

namespace anyvalueeditor
{

struct DomainAnyValueBuilder::DomainAnyValueBuilderImpl
{
  const AnyValueItem &m_item;
  AnyValueBuildAdapter m_build_adapter;

  explicit DomainAnyValueBuilderImpl(const AnyValueItem &item) : m_item(item) {}

  void ProcessItem() {}
};

DomainAnyValueBuilder::DomainAnyValueBuilder(const AnyValueItem &item)
    : p_impl(std::make_unique<DomainAnyValueBuilderImpl>(item))
{
  p_impl->ProcessItem();
}

DomainAnyValueBuilder::~DomainAnyValueBuilder() = default;

sup::dto::AnyValue DomainAnyValueBuilder::GetAnyValue() const
{
  return p_impl->m_build_adapter.MoveAnyValue();
}

}  // namespace anyvalueeditor
