/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#include "anyvalue_editor_dialog_factory.h"

#include "anyvalue_compact_scalar_editor.h"
#include "anyvalue_compact_tree_editor.h"

#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_dialog.h>

namespace sequencergui
{

std::unique_ptr<sup::gui::AnyValueEditorDialog> CreateAnyValueExtendedEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent)
{
  auto editor = std::make_unique<sup::gui::AnyValueEditor>();
  editor->SetInitialValue(item);
  editor->setWindowTitle("AnyValueExtendedEditor");
  return std::make_unique<sup::gui::AnyValueEditorDialog>(std::move(editor), parent);
}

std::unique_ptr<sup::gui::AnyValueEditorDialog> CreateAnyValueCompactTreeEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent)
{
  auto editor = std::make_unique<AnyValueCompactTreeEditor>();
  editor->SetInitialValue(item);
  return std::make_unique<sup::gui::AnyValueEditorDialog>(std::move(editor), parent);
}

std::unique_ptr<sup::gui::AnyValueEditorDialog> CreateAnyValueCompactScalarEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent)
{
  auto editor = std::make_unique<AnyValueCompactScalarEditor>();
  editor->SetInitialValue(item);
  return std::make_unique<sup::gui::AnyValueEditorDialog>(std::move(editor), parent);
}

std::function<AnyValueDialogResult(const sup::gui::AnyValueItem*)> CreateAnyValueDialogCallback(
    QWidget* parent)
{
  auto edit_anyvalue_callback = [parent](const sup::gui::AnyValueItem* item) -> AnyValueDialogResult
  {
    auto dialog = CreateAnyValueExtendedEditorDialog(item, parent);
    if (dialog->exec() == QDialog::Accepted)
    {
      return {true, dialog->GetResult()};
    }
    return {false, {}};  // returning false denotes that dialog was canceled
  };

  return edit_anyvalue_callback;
}

}  // namespace sequencergui
