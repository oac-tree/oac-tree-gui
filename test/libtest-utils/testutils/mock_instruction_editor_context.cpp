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

#include "mock_instruction_editor_context.h"

#include <oac_tree_gui/composer/instruction_editor_action_handler.h>
#include <oac_tree_gui/model/instruction_container_item.h>

#include <sup/gui/model/anyvalue_item.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

InstructionEditorContext MockInstructionEditorContext::CreateContext(
    InstructionContainerItem* instruction_container,
    const std::vector<InstructionItem*>& current_selection)
{
  InstructionEditorContext result;

  m_current_selection = current_selection;

  result.instruction_container = [this, instruction_container]() { return instruction_container; };
  result.selected_instructions = [this]() { return m_current_selection; };
  result.select_notify = [this](auto item) { SelectRequest(item); };
  result.send_message = [this](const auto& message) { OnMessage(message); };
  result.edit_anyvalue = [this](const sup::gui::AnyValueItem* item)
  { return OnEditAnyvalue(item); };
  result.get_mime_data = [this]()
  {
    OnGetMimeData();
    return m_clipboard_content.get();
  };
  result.set_mime_data = [this](auto mime_data)
  {
    // mimicking clipboard, saving copy result here
    m_clipboard_content = std::move(mime_data);
    OnSetMimeData();
  };

  return result;
}

std::unique_ptr<InstructionEditorActionHandler> MockInstructionEditorContext::CreateActionHandler(
    InstructionContainerItem* instruction_container,
    const std::vector<InstructionItem *> &current_selection)
{
  return std::make_unique<InstructionEditorActionHandler>(
      CreateContext(instruction_container, current_selection));
}

QMimeData* MockInstructionEditorContext::GetClipboardContent() const
{
  return m_clipboard_content.get();
}

void MockInstructionEditorContext::SetClipboardContent(std::unique_ptr<QMimeData> clipboard)
{
  m_clipboard_content = std::move(clipboard);
}

}  // namespace oac_tree_gui::test
