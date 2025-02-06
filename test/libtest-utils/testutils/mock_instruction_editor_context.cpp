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

#include <sup/gui/model/anyvalue_item.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

InstructionEditorContext MockInstructionEditorContext::CreateContext(ProcedureItem* procedure,
                                                                     InstructionItem* instruction)
{
  InstructionEditorContext result;

  result.selected_procedure = [this, procedure]() { return procedure; };
  result.selected_instruction = [this, instruction]() { return instruction; };
  result.send_message = [this](const auto& message) { OnMessage(message); };
  result.edit_anyvalue_callback = [this](const sup::gui::AnyValueItem* item)
  { return OnEditAnyvalue(item); };
  result.get_mime_data = [this]() { return OnGetMimeData(); };
  result.set_mime_data = [this](auto mime_data) { OnSetMimeData(std::move(mime_data)); };

  return result;
}

std::unique_ptr<InstructionEditorActionHandler> MockInstructionEditorContext::CreateActionHandler(
    ProcedureItem* procedure, InstructionItem* instruction)
{
  return std::make_unique<InstructionEditorActionHandler>(CreateContext(procedure, instruction));
}

}  // namespace oac_tree_gui::test
