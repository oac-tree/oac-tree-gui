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

#ifndef SEQUENCERGUI_MODEL_AGGREGATE_EXAMPLES_H_
#define SEQUENCERGUI_MODEL_AGGREGATE_EXAMPLES_H_

//! Collection of factory functions to create complex instructions.

#include <memory>

namespace sequencergui
{
class InstructionItem;

namespace Examples
{

//! Creates an aggregate representing  if-then-else statement
std::unique_ptr<InstructionItem> CreateIfThenElseAggregate();

}  // namespace Examples
}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_AGGREGATE_EXAMPLES_H_
