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

#include "anyvalueeditor/anyvalue_item.h"
#include "anyvalueeditor/transform_from_anyvalue.h"

#include "mvvm/model/application_model.h"
#include "mvvm/model/sessionitem.h"
#include "mvvm/model/tagindex.h"

#include <iostream>

namespace
{
template <typename T>
T *InsertItem(mvvm::SessionItem *parent, const mvvm::TagIndex &tag_index)
{
  if (auto model = parent->GetModel(); model)
  {
    return model->InsertItem<T>(parent, tag_index);
  }
  else
  {
    return parent->InsertItem<T>(tag_index);
  }
}

}  // namespace

namespace anyvalueeditor
{

AnyValueItemBuilder::AnyValueItemBuilder(AnyValueItem *item) : m_item(item) {}

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
//  std::cout << "AddStructProlog() value:" << anyvalue << " item:" << m_item << std::endl;
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

void AnyValueItemBuilder::MemberProlog(const anyvalue_t *anyvalue,
                                          const std::string &member_name)
{
  (void)anyvalue;
//  std::cout << "AddMemberProlog() " << m_item << " " << member_name << std::endl;
  auto child = InsertItem<AnyValueItem>(m_item, mvvm::TagIndex::Append());
  child->SetDisplayName(member_name);
  m_item = child;
}

void AnyValueItemBuilder::MemberEpilog(const anyvalue_t *anyvalue,
                                          const std::string &member_name)
{
  (void)anyvalue;
//  std::cout << "AddMemberEpilog() " << m_item << " " << member_name << std::endl;
  m_item = static_cast<AnyValueItem *>(m_item->GetParent());
}

void AnyValueItemBuilder::ArrayProlog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddArrayProlog() value:" << anyvalue << " item:" << m_item << std::endl;
  auto child = InsertItem<AnyValueItem>(m_item, mvvm::TagIndex::Append());
  m_item = child;
  m_index = 0;
  child->SetDisplayName("index" + std::to_string(m_index++));
}

void AnyValueItemBuilder::ArrayElementSeparator()
{
//  std::cout << "AddArrayElementSeparator() "
//            << " item:" << m_item << std::endl;
  m_item = static_cast<AnyValueItem *>(m_item->GetParent());

  auto child = InsertItem<AnyValueItem>(m_item, mvvm::TagIndex::Append());
  child->SetDisplayName("index" + std::to_string(m_index++));
  m_item = child;
}

void AnyValueItemBuilder::ArrayEpilog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddArrayEpilog() value:" << anyvalue << " item:" << m_item << std::endl;
  m_item = static_cast<AnyValueItem *>(m_item->GetParent());
}

void AnyValueItemBuilder::ScalarProlog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddScalarProlog() value:" << anyvalue << " item:" << m_item << std::endl;
}

void AnyValueItemBuilder::ScalarEpilog(const anyvalue_t *anyvalue)
{
//  std::cout << "AddScalarEpilog() value:" << anyvalue << " item:" << m_item << std::endl;
  ScalarToItem(*anyvalue, *m_item);
}

void AnyValueItemBuilder::UnboundedArrayProlog(const anyvalue_t *val)
{

}

void AnyValueItemBuilder::UnboundedArrayEpilog(const anyvalue_t *val)
{

}

}  // namespace anyvalueeditor

#include "anyvalueeditor/anyvalue_item_builder.h"
