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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "aggregate_examples.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

namespace oac_tree_gui::Examples
{

std::unique_ptr<InstructionItem> CreateIfThenElseAggregate()
{
  auto result = std::make_unique<UniversalInstructionItem>();
  result->SetDomainType(domainconstants::kFallbackInstructionType);

  auto sequence0 = result->InsertItem<UniversalInstructionItem>(::mvvm::TagIndex::Append());
  sequence0->SetDomainType(domainconstants::kSequenceInstructionType);
  (void)sequence0->InsertItem<IncludeItem>(::mvvm::TagIndex::Append())->SetDisplayName("A");

  auto force_success_item =
      sequence0->InsertItem<UniversalInstructionItem>(::mvvm::TagIndex::Append());
  force_success_item->SetDomainType(domainconstants::kForceSuccessInstructionType);
  (void)force_success_item->InsertItem<IncludeItem>(::mvvm::TagIndex::Append())->SetDisplayName("B");

  (void)result->InsertItem<IncludeItem>(::mvvm::TagIndex::Append())->SetDisplayName("C");

  return result;
}

}  // namespace oac_tree_gui::Examples
