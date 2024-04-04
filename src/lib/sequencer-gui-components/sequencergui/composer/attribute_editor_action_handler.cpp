/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sup/gui/model/anyvalue_item.h>

/**
 * @brief Returns currently selected attribute item.
 */
namespace sequencergui
{

AttributeEditorActionHandler::AttributeEditorActionHandler(AttributeEditorContext context,
                                                           QObject *parent)
    : QObject(parent), m_context(std::move(context))
{
}

AttributeEditorActionHandler::~AttributeEditorActionHandler() = default;

bool AttributeEditorActionHandler::CanToggleEnabledFlag() const
{
  return false;
}

void AttributeEditorActionHandler::OnToggleEnabledFlag()
{
  if (!CanToggleEnabledFlag())
  {
    return;
  }
}

bool AttributeEditorActionHandler::CanSetDefaultType() const
{
  return false;
}

void AttributeEditorActionHandler::OnSetAsDefaultType()
{
  if (!CanSetDefaultType())
  {
    return;
  }
}

bool AttributeEditorActionHandler::CanSetPlaceholderType() const
{
  return false;
}

void AttributeEditorActionHandler::OnSetPlaceholderType()
{
  if (!CanSetPlaceholderType())
  {
    return;
  }
}

bool AttributeEditorActionHandler::CanEditAnyValue() const
{
  return false;
}

sup::gui::AnyValueItem *AttributeEditorActionHandler::GetSelectedAttributeItem() const
{
  return m_context.selected_item_callback ? dynamic_cast<sup::gui::AnyValueItem *>(m_context.selected_item_callback())
                   : nullptr;
}

}  // namespace sequencergui
