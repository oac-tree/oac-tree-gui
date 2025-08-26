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

TextEditController::TextEditController(const TextControllerContext& context)
    : m_context(context)
    , m_text_edit_connection(std::make_unique<QMetaObject::Connection>())
    , m_checkbox_connection(std::make_unique<QMetaObject::Connection>())
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
  Listener()->Connect<mvvm::PropertyChangedEvent>(this,
                                                  &TextEditController::OnPropertyChangedEvent);

  UpdateWidgetStateFromItem();
  SetQtConnected();
}

void TextEditController::Unsubscribe()
{
  SetQtDisonnected();
}

void TextEditController::SetQtConnected()
{
  auto on_text_changed = [this]()
  {
    if (!GetItem())
    {
      return;
    }

    const auto text = m_context.text_edit->toPlainText();
    auto trimmed = mvvm::utils::TrimWhitespace(text.toStdString());
    auto lines = mvvm::utils::SplitString(trimmed, "\n");

    m_do_not_update_widgets = true;
    GetItem()->SetText(lines);
    m_do_not_update_widgets = false;
  };

  *m_text_edit_connection =
      QObject::connect(m_context.text_edit, &QTextEdit::textChanged, on_text_changed);

  auto on_checkstate_changed = [this](int state)
  {
    if (!GetItem())
    {
      return;
    }

    const bool enabled = (state == Qt::Checked);
    m_do_not_update_widgets = true;
    GetItem()->SetEditorEnabled(enabled);
    m_context.text_edit->setEnabled(enabled);
    m_do_not_update_widgets = false;
  };

  *m_checkbox_connection =
      QObject::connect(m_context.check_box, &QCheckBox::checkStateChanged, on_checkstate_changed);
}

void TextEditController::SetQtDisonnected()
{
  QObject::disconnect(*m_text_edit_connection);
  QObject::disconnect(*m_checkbox_connection);
}

void TextEditController::OnPropertyChangedEvent(const mvvm::PropertyChangedEvent& event)
{
  (void)event;
  SetQtDisonnected();
  UpdateWidgetStateFromItem();
  SetQtConnected();
}

void TextEditController::UpdateWidgetStateFromItem()
{
  if (m_do_not_update_widgets)
  {
    return;
  }

  m_context.check_box->setChecked(GetItem()->IsEditorEnabled());
  m_context.text_edit->setEnabled(m_context.check_box->isChecked());
  m_context.text_edit->setPlainText(GetText(GetItem()->GetText()));
}

}  // namespace oac_tree_gui
