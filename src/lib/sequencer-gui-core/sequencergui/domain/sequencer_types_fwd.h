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

#ifndef SEQUENCERGUI_DOMAIN_SEQUENCER_TYPES_H_
#define SEQUENCERGUI_DOMAIN_SEQUENCER_TYPES_H_

//! Type aliases for main Sequencer types.

namespace sup::sequencer
{
class Procedure;
class Instruction;
class Variable;
class Runner;
class UserInterface;
class Workspace;
}  // namespace sup::sequencer

using procedure_t = sup::sequencer::Procedure;
using instruction_t = sup::sequencer::Instruction;
using variable_t = sup::sequencer::Variable;
using runner_t = sup::sequencer::Runner;
using userinterface_t = sup::sequencer::UserInterface;
using workspace_t = sup::sequencer::Workspace;

#endif  // SEQUENCERGUI_DOMAIN_SEQUENCER_TYPES_H_
