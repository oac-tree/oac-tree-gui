/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/universal_item_helper.h>
#include <oac_tree_gui/transform/instruction_item_transform_helper.h>

namespace oac_tree_gui::Examples
{

std::unique_ptr<InstructionItem> CreateIfThenElseAggregate()
{
  auto result = CreateInstructionItem(domainconstants::kFallbackInstructionType);

  auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, result.get());
  (void)InsertInstruction(domainconstants::kIncludeInstructionType, sequence0)->SetDisplayName("A");

  auto force_success_item =
      InsertInstruction(domainconstants::kForceSuccessInstructionType, sequence0);
  (void)InsertInstruction(domainconstants::kIncludeInstructionType, force_success_item)
      ->SetDisplayName("B");

  (void)InsertInstruction(domainconstants::kIncludeInstructionType, result.get())
      ->SetDisplayName("C");

  return result;
}

}  // namespace oac_tree_gui::Examples
