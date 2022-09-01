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
  std::cout << "AddEmptyProlog() value:" << anyvalue << " item:" << m_current_item << std::endl;
}

void AnyValueItemBuilder::EmptyEpilog(const anyvalue_t *anyvalue)
{
  std::cout << "AddEmptyEpilog() value:" << anyvalue << " item:" << m_current_item << std::endl;
}

void AnyValueItemBuilder::StructProlog(const anyvalue_t *anyvalue)
{
  std::cout << "AddStructProlog() value:" << anyvalue << " item:" << m_result.get() << std::endl;
  auto struct_item = std::make_unique<AnyValueStructItem>();
  auto struct_item_ptr = struct_item.get();

  if (!m_result)
  {
    m_result = std::move(struct_item);
  }
  else if (!m_member_name.empty())
  {
    struct_item->SetDisplayName(m_member_name);
    m_current_item->InsertItem(std::move(struct_item), mvvm::TagIndex::Append());
  }
  else
  {
    throw std::logic_error("Error in logic");
  }

  m_current_item = struct_item_ptr;
}

void AnyValueItemBuilder::StructMemberSeparator()
{
  std::cout << "AddStructMemberSeparator() "
            << " item:" << m_current_item << std::endl;
}

void AnyValueItemBuilder::StructEpilog(const anyvalue_t *anyvalue)
{
  std::cout << "AddStructEpilog() value:" << anyvalue << " item:" << m_current_item << std::endl;
}

//! Append new child with the display name corresponding to `member_name`.
//! Update

void AnyValueItemBuilder::MemberProlog(const anyvalue_t *anyvalue, const std::string &member_name)
{
  m_member_name = member_name;
  //  (void)anyvalue;
  //  std::cout << "AddMemberProlog() " << m_current_item << " " << member_name << std::endl;
  //  auto child = m_current_item->InsertItem<AnyValueItem>(mvvm::TagIndex::Append());
  //  child->SetDisplayName(member_name);
  //  m_current_item = child;
}

void AnyValueItemBuilder::MemberEpilog(const anyvalue_t *anyvalue, const std::string &member_name)
{
  (void)anyvalue;
  std::cout << "AddMemberEpilog() " << m_current_item << " " << member_name << std::endl;
  m_member_name.clear();
  m_current_item = static_cast<AnyValueItem *>(m_current_item->GetParent());
}

void AnyValueItemBuilder::ArrayProlog(const anyvalue_t *anyvalue)
{
  std::cout << "AddArrayProlog() value:" << anyvalue << " item:" << m_current_item << std::endl;

  auto array_item = std::make_unique<AnyValueArrayItem>();
  auto array_item_ptr = array_item.get();

  if (!m_result)
  {
    m_result = std::move(array_item);
  }
  else if (!m_member_name.empty())
  {
    array_item_ptr->SetDisplayName(m_member_name);
    m_current_item->InsertItem(std::move(array_item), mvvm::TagIndex::Append());
  }
  else
  {
    throw std::logic_error("Error in logic");
  }

  //  child->SetDisplayName("index" + std::to_string(m_index++));

  m_index = 0;
  m_current_item = array_item_ptr;
}

void AnyValueItemBuilder::ArrayElementSeparator()
{
  std::cout << "AddArrayElementSeparator() "
            << " item:" << m_current_item << std::endl;

  m_index++;
  //  m_current_item = static_cast<AnyValueItem *>(m_current_item->GetParent());

  //  auto child = m_current_item->InsertItem<AnyValueItem>(mvvm::TagIndex::Append());
  //  child->SetDisplayName("index" + std::to_string(m_index++));
  //  m_current_item = child;
}

void AnyValueItemBuilder::ArrayEpilog(const anyvalue_t *anyvalue)
{
  std::cout << "AddArrayEpilog() value:" << anyvalue << " item:" << m_current_item << std::endl;
  m_index = -1;
  m_current_item = static_cast<AnyValueItem *>(m_current_item->GetParent());
}

void AnyValueItemBuilder::ScalarProlog(const anyvalue_t *anyvalue)
{
  std::cout << "AddScalarProlog() value:" << anyvalue << " item:" << m_current_item << std::endl;

  auto scalar = std::make_unique<AnyValueScalarItem>();
  auto scalar_ptr = scalar.get();
  scalar->SetAnyTypeName(anyvalue->GetTypeName());
  SetDataFromScalar(*anyvalue, *scalar);

  if (!m_result)
  {
    m_result = std::move(scalar);
  }
  else if (!m_member_name.empty())
  {
    scalar_ptr->SetDisplayName(m_member_name);
    m_current_item->InsertItem(std::move(scalar), mvvm::TagIndex::Append());
  }
  else
  {
    throw std::logic_error("Error in logic");
  }

  m_current_item = scalar_ptr;
}

void AnyValueItemBuilder::ScalarEpilog(const anyvalue_t *anyvalue)
{
  std::cout << "AddScalarEpilog() value:" << anyvalue << " item:" << m_current_item << std::endl;
}

void AnyValueItemBuilder::AddItem(std::unique_ptr<AnyValueItem> item)
{
  auto item_ptr = item.get();

  if (!m_result)
  {
    m_result = std::move(item);
  }
  else if (!m_member_name.empty())
  {
    m_current_item->InsertItem(std::move(item), mvvm::TagIndex::Append())
        ->SetDisplayName(m_member_name);
  }
  else
  {
    throw std::logic_error("Error in logic");
  }

  m_current_item = item_ptr;
}

}  // namespace anyvalueeditor
