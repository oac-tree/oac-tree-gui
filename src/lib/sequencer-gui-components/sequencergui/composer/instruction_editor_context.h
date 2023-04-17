/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_CONTEXT_H_
#define SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_CONTEXT_H_

#include <functional>

namespace sequencergui
{

class ProcedureItem;
class InstructionItem;
class VariableItem;

//! Context representing current state of widgets related to the instruction composition.
//! Allows to retrieve currently selected procedure and instruction.

struct InstructionEditorContext
{
  //! Callback to get currently selected procedure.
  std::function<ProcedureItem*()> selected_procedure;

  //! Callback to get currently selected instruction.
  std::function<InstructionItem*()> selected_instruction;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTION_EDITOR_CONTEXT_H_
