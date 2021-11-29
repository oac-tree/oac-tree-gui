/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef MVVM_SEQUENCER_SEQUENCER_TYPES_H
#define MVVM_SEQUENCER_SEQUENCER_TYPES_H

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

#endif  // MVVM_SEQUENCER_SEQUENCER_TYPES_H
