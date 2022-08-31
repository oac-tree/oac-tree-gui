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

#include "anyvalueeditor/anyvalue_item_builder.h"

#include <anyvalueeditor/anyvalue_item.h>
#include <anyvalueeditor/anyvalue_utils.h>
#include <anyvalueeditor/scalar_conversion_utils.h>
#include <mvvm/model/application_model.h>
#include <mvvm/model/sessionitem.h>
#include <mvvm/model/tagindex.h>
#include <sup/dto/anyvalue.h>

#include <iostream>

namespace anyvalueeditor
{

std::unique_ptr<AnyValueItem> AnyValueItemBuilder::MoveAnyValueItem()
{
  return std::move(m_result);
}

void AnyValueItemBuilder::EmptyProlog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddEmptyProlog() value:" << anyvalue << " item:" << m_item << std::endl;
}

void AnyValueItemBuilder::EmptyEpilog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddEmptyEpilog() value:" << anyvalue << " item:" << m_item << std::endl;
}

void AnyValueItemBuilder::StructProlog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddStructProlog() value:" << anyvalue << " item:" << m_result.get() << std::endl;
  if (!m_result)
  {
    m_result = std::make_unique<AnyValueItem>();
    m_result->SetAnyTypeName(kStructTypeName);
    m_current_parent = m_result.get();
  }
}

void AnyValueItemBuilder::StructMemberSeparator()
{
//  std::cout << "AddStructMemberSeparator() "
//            << " item:" << m_item << std::endl;
}

void AnyValueItemBuilder::StructEpilog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddStructEpilog() value:" << anyvalue << " item:" << m_item << std::endl;
}

//! Append new child with the display name corresponding to `member_name`.
//! Update

void AnyValueItemBuilder::MemberProlog(const anyvalue_t *anyvalue, const std::string &member_name)
{
  (void)anyvalue;
//  std::cout << "AddMemberProlog() " << m_item << " " << member_name << std::endl;
  auto child = m_current_parent->InsertItem<AnyValueItem>(mvvm::TagIndex::Append());
  child->SetDisplayName(member_name);
  m_current_parent = child;
}

void AnyValueItemBuilder::MemberEpilog(const anyvalue_t *anyvalue, const std::string &member_name)
{
  (void)anyvalue;
//  std::cout << "AddMemberEpilog() " << m_item << " " << member_name << std::endl;
  m_current_parent = static_cast<AnyValueItem *>(m_current_parent->GetParent());
}

void AnyValueItemBuilder::ArrayProlog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddArrayProlog() value:" << anyvalue << " item:" << m_item << std::endl;
  if (!m_result)
  {
    m_result = std::make_unique<AnyValueItem>();
    m_current_parent = m_result.get();
  }

  auto child = m_current_parent->InsertItem<AnyValueItem>(mvvm::TagIndex::Append());
  m_current_parent = child;
  m_index = 0;
  child->SetDisplayName("index" + std::to_string(m_index++));
}

void AnyValueItemBuilder::ArrayElementSeparator()
{
//  std::cout << "AddArrayElementSeparator() "
//            << " item:" << m_item << std::endl;
  m_current_parent = static_cast<AnyValueItem *>(m_current_parent->GetParent());

  auto child = m_current_parent->InsertItem<AnyValueItem>(mvvm::TagIndex::Append());
  child->SetDisplayName("index" + std::to_string(m_index++));
  m_current_parent = child;
}

void AnyValueItemBuilder::ArrayEpilog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddArrayEpilog() value:" << anyvalue << " item:" << m_item << std::endl;
  m_current_parent = static_cast<AnyValueItem *>(m_current_parent->GetParent());
}

void AnyValueItemBuilder::ScalarProlog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddScalarProlog() value:" << anyvalue << " item:" << m_item << std::endl;
}

void AnyValueItemBuilder::ScalarEpilog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddScalarEpilog() value:" << anyvalue << " item:" << m_item << std::endl;

  if (!m_result)
  {
    m_result = std::make_unique<AnyValueItem>();
    m_current_parent = m_result.get();
  }

  m_current_parent->SetAnyTypeName(anyvalue->GetTypeName());
  SetDataFromScalar(*anyvalue, *m_current_parent);
}

}  // namespace anyvalueeditor
