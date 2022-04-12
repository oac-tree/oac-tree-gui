/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_COMPOSERCONTEXT_H
#define SEQUENCERGUI_COMPOSER_COMPOSERCONTEXT_H

#include <functional>

namespace sequencergui
{

class ProcedureItem;
class InstructionItem;
class VariableItem;

//! Context representing current state of widgets related to the procedure composition.
//! Allows to retrieve currently selected procedure, instruction and variable.

struct ComposerContext
{
  //! Callback to get currently selected procedure.
  std::function<ProcedureItem*()> selected_procedure;

  //! Callback to get currently selected instruction.
  std::function<InstructionItem*()> selected_instruction;

  //! Callback to get currently selected variable.
  std::function<VariableItem*()> selected_variable;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_COMPOSERCONTEXT_H
