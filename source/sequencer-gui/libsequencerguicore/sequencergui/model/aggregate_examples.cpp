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

#include "sequencergui/model/aggregate_examples.h"

#include <sequencergui/model/standard_instruction_items.h>

namespace sequencergui::Examples
{

std::unique_ptr<InstructionItem> CreateIfThenElseAggregate()
{
  auto result = std::make_unique<FallbackItem>();
  result->InsertItem<SequenceItem>(::mvvm::TagIndex::Append());
  return result;
}

}  // namespace sequencergui::Examples
