/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "attribute_editor_action_handler.h"

#include <oac_tree_gui/model/attribute_item.h>
#include <oac_tree_gui/model/item_constants.h>

#include <sup/gui/model/anyvalue_item.h>

namespace oac_tree_gui
{

AttributeEditorActionHandler::AttributeEditorActionHandler(AttributeEditorContext context)
    : m_context(std::move(context))
{
}

AttributeEditorActionHandler::~AttributeEditorActionHandler() = default;

bool AttributeEditorActionHandler::CanSetPlaceholderType() const
{
  const auto* attribute = GetSelectedAttribute();
  return (attribute != nullptr) && attribute->IsTextAllowed();
}

void AttributeEditorActionHandler::OnSetPlaceholderType()
{
  if (auto* attribute = GetSelectedAttribute();
      (attribute != nullptr) && attribute->IsTextAllowed())
  {
    (void)attribute->SetText(itemconstants::kDefaultPlaceholderAttributeValue);
  }
}

bool AttributeEditorActionHandler::CanEditAnyValue() const
{
  return IsAnyValue();
}

mvvm::SessionItem* AttributeEditorActionHandler::GetSelectedItem() const
{
  if (m_context.selected_items_callback)
  {
    auto selected_items = m_context.selected_items_callback();
    return selected_items.empty() ? nullptr : selected_items.front();
  }

  return nullptr;
}

AttributeItem* AttributeEditorActionHandler::GetSelectedAttribute() const
{
  return dynamic_cast<AttributeItem*>(GetSelectedItem());
}

bool AttributeEditorActionHandler::IsAnyValue() const
{
  const auto* item = dynamic_cast<sup::gui::AnyValueItem*>(GetSelectedItem());
  return (item != nullptr) && item->GetTagIndex().GetTag() == itemconstants::kAnyValueTag;
}

}  // namespace oac_tree_gui
