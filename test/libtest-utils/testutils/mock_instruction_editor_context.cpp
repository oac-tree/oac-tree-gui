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

#include <sup/gui/model/anyvalue_item.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

InstructionEditorContext MockInstructionEditorContext::CreateContext()
{
  InstructionEditorContext result;

  result.selected_procedure = [this]() { return OnSelectedProcedure(); };
  result.selected_instruction = [this]() { return OnSelectedInstruction(); };
  result.send_message_callback = [this](const auto& message) { OnMessage(message); };
  result.get_mime_data = [this]() { return OnGetMimeData(); };
  result.set_mime_data = [this](auto mime_data) { OnSetMimeData(std::move(mime_data)); };

  return result;
}

}  // namespace oac_tree_gui::test
