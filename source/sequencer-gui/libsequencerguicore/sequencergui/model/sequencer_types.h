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

#ifndef SEQUENCERGUI_MODEL_SEQUENCER_TYPES_H
#define SEQUENCERGUI_MODEL_SEQUENCER_TYPES_H

//! @file sequencer_types.h
//! Type aliases for main Sequencer types.

namespace sup::sequencer
{
class Procedure;
class Instruction;
class Variable;
class Runner;
}  // namespace sup::sequencer

using procedure_t = sup::sequencer::Procedure;
using instruction_t = sup::sequencer::Instruction;
using variable_t = sup::sequencer::Variable;
using runner_t = sup::sequencer::Runner;

#endif  // SEQUENCERGUI_MODEL_SEQUENCER_TYPES_H
