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
#include <oac_tree_gui/transform/attribute_item_transform_helper.h>

#include <sup/gui/model/anyvalue_item.h>

namespace oac_tree_gui
{

AttributeEditorActionHandler::AttributeEditorActionHandler(AttributeEditorContext context)
    : m_context(std::move(context))
{
}

AttributeEditorActionHandler::~AttributeEditorActionHandler() = default;

bool AttributeEditorActionHandler::CanToggleExposedFlag() const
{
  if (const auto* attribute = GetSelectedAttribute(); attribute != nullptr)
  {
    return attribute->HasFlag(mvvm::Appearance::kNullable);
  }
  return GetSelectedAnyValueItem() != nullptr;
}

bool AttributeEditorActionHandler::GetExposedFlag() const
{
  if (const auto* attribute = GetSelectedAttribute(); attribute != nullptr)
  {
    return attribute->HasData();
  }
  return (GetSelectedAnyValueItem() != nullptr)
         && GetAttributeExposedFlag(*GetSelectedAnyValueItem());
}

void AttributeEditorActionHandler::OnToggleExposedFlag()
{
  if (!CanToggleExposedFlag())
  {
    return;
  }

  if (auto* attribute = GetSelectedAttribute(); attribute != nullptr)
  {
    (void)attribute->SetData(attribute->HasData() ? mvvm::variant_t{}
                                                  : attribute->GetDefaultValue());
    return;
  }

  auto* anyvalue_item = GetSelectedAnyValueItem();
  SetAttributeExposedFlag(!GetAttributeExposedFlag(*anyvalue_item), *anyvalue_item);
}

bool AttributeEditorActionHandler::CanSetDefaultType() const
{
  return (GetSelectedAttribute() != nullptr)
         || ((GetSelectedAnyValueItem() != nullptr) && !IsAnyValue());
}

void AttributeEditorActionHandler::OnSetAsDefaultType()
{
  if (!CanSetDefaultType())
  {
    return;
  }

  if (auto* attribute = GetSelectedAttribute(); attribute != nullptr)
  {
    (void)attribute->SetData(attribute->GetDefaultValue());
    return;
  }

  SetAttributeFromTypeName(*GetSelectedAnyValueItem());
}

bool AttributeEditorActionHandler::CanSetPlaceholderType() const
{
  return CanSetDefaultType();
}

void AttributeEditorActionHandler::OnSetPlaceholderType()
{
  if (!CanSetPlaceholderType())
  {
    return;
  }

  if (auto* attribute = GetSelectedAttribute(); attribute != nullptr)
  {
    (void)attribute->SetData(std::string(itemconstants::kDefaultPlaceholderAttributeValue));
    return;
  }

  SetAttributeAsString(itemconstants::kDefaultPlaceholderAttributeValue,
                       *GetSelectedAnyValueItem());
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

sup::gui::AnyValueItem* AttributeEditorActionHandler::GetSelectedAnyValueItem() const
{
  return dynamic_cast<sup::gui::AnyValueItem*>(GetSelectedItem());
}

bool AttributeEditorActionHandler::IsAnyValue() const
{
  return (GetSelectedAnyValueItem() != nullptr)
         && GetSelectedAnyValueItem()->GetTagIndex().GetTag() == itemconstants::kAnyValueTag;
}

}  // namespace oac_tree_gui
