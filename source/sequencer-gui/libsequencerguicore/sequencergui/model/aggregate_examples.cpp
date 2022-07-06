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
  result->SetDisplayName("IfThenElse");

  auto sequence0 = result->InsertItem<SequenceItem>(::mvvm::TagIndex::Append());
  auto include0 =
      sequence0->InsertItem<IncludeItem>(::mvvm::TagIndex::Append())->SetDisplayName("A");
  auto force_success_item = sequence0->InsertItem<ForceSuccessItem>(::mvvm::TagIndex::Append());
  auto include1 =
      force_success_item->InsertItem<IncludeItem>(::mvvm::TagIndex::Append())->SetDisplayName("B");

  auto include2 = result->InsertItem<IncludeItem>(::mvvm::TagIndex::Append())->SetDisplayName("C");

  return result;
}

}  // namespace sequencergui::Examples
