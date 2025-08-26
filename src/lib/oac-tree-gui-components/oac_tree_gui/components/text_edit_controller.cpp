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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "text_edit_controller.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/text_edit_item.h>

#include <mvvm/utils/string_utils.h>

#include <QCheckBox>
#include <QTextEdit>

namespace oac_tree_gui
{

/**
 * @brief Returns multiline Qt string made of separate std::strings.
 */
QString GetText(const std::vector<std::string>& lines)
{
  auto result = QString::fromStdString(mvvm::utils::VectorToString(lines, "\n"));
  if (!result.isEmpty())
  {
    result.append("\n");
  }
  return result;
}

TextEditController::TextEditController(const TextControllerContext& context) : m_context(context)
{
  if (!context.check_box)
  {
    throw RuntimeException("QCheckBox is not initialized");
  }

  if (!context.text_edit)
  {
    throw RuntimeException("QTextEdit is not initialized");
  }
}

void TextEditController::Subscribe()
{
  UpdateWidgetStateToItem();
}

void TextEditController::UpdateWidgetStateToItem()
{
  m_context.check_box->setChecked(GetItem()->IsEditorEnabled());
  m_context.text_edit->setEnabled(m_context.check_box->isChecked());
  m_context.text_edit->setPlainText(GetText(GetItem()->GetText()));
}

}  // namespace oac_tree_gui
