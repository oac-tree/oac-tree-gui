/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "attribute_editor_action_handler.h"

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
  return GetSelectedAttributeItem() != nullptr;
}

bool AttributeEditorActionHandler::GetExposedFlag() const
{
  return GetSelectedAttributeItem() && GetAttributeExposedFlag(*GetSelectedAttributeItem());
}

void AttributeEditorActionHandler::OnToggleExposedFlag()
{
  if (!CanToggleExposedFlag())
  {
    return;
  }

  auto attribute_item = GetSelectedAttributeItem();
  SetAttributeExposedFlag(!GetAttributeExposedFlag(*attribute_item), *attribute_item);
}

bool AttributeEditorActionHandler::CanSetDefaultType() const
{
  return GetSelectedAttributeItem() != nullptr && !IsAnyValue();
}

void AttributeEditorActionHandler::OnSetAsDefaultType()
{
  if (!CanSetDefaultType())
  {
    return;
  }
  SetAttributeFromTypeName(*GetSelectedAttributeItem());
}

bool AttributeEditorActionHandler::CanSetPlaceholderType() const
{
  return GetSelectedAttributeItem() != nullptr && !IsAnyValue();
}

void AttributeEditorActionHandler::OnSetPlaceholderType()
{
  if (!CanSetPlaceholderType())
  {
    return;
  }

  SetAttributeAsString(itemconstants::kDefaultPlaceholderAttributeValue,
                       *GetSelectedAttributeItem());
}

bool AttributeEditorActionHandler::CanEditAnyValue() const
{
  return IsAnyValue();
}

sup::gui::AnyValueItem *AttributeEditorActionHandler::GetSelectedAttributeItem() const
{
  if (m_context.selected_items_callback)
  {
    auto selected_items = m_context.selected_items_callback();
    return selected_items.empty() ? nullptr
                                  : dynamic_cast<sup::gui::AnyValueItem *>(selected_items.front());
  }

  return nullptr;
}

bool AttributeEditorActionHandler::IsAnyValue() const
{
  return GetSelectedAttributeItem()
         && GetSelectedAttributeItem()->GetTagIndex().GetTag() == itemconstants::kAnyValueTag;
}

}  // namespace oac_tree_gui
