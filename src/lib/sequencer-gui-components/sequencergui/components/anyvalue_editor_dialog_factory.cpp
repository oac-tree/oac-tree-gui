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

#include "anyvalue_editor_dialog_factory.h"

#include "abstract_anyvalue_editor.h"
#include "anyvalue_compact_scalar_editor.h"
#include "anyvalue_compact_tree_editor.h"
#include "anyvalue_editor_dialog.h"
#include "anyvalue_extended_editor.h"

namespace sequencergui
{

std::unique_ptr<AnyValueEditorDialog> CreateAnyValueExtendedEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent)
{
  auto editor = std::make_unique<AnyValueExtendedEditor>();
  editor->SetInitialValue(item);
  return std::make_unique<AnyValueEditorDialog>(std::move(editor), parent);
}

std::unique_ptr<AnyValueEditorDialog> CreateAnyValueCompactTreeEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent)
{
  auto editor = std::make_unique<AnyValueCompactTreeEditor>();
  editor->SetInitialValue(item);
  return std::make_unique<AnyValueEditorDialog>(std::move(editor), parent);
}

std::unique_ptr<AnyValueEditorDialog> CreateAnyValueCompactScalarEditorDialog(
    const sup::gui::AnyValueItem* item, QWidget* parent)
{
  auto editor = std::make_unique<AnyValueCompactScalarEditor>();
  editor->SetInitialValue(item);
  return std::make_unique<AnyValueEditorDialog>(std::move(editor), parent);
}

}  // namespace sequencergui
