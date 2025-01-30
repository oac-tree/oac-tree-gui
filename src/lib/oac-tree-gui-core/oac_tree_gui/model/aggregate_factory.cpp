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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "aggregate_factory.h"

#include <oac_tree_gui/model/aggregate_examples.h>
#include <oac_tree_gui/model/instruction_item.h>

namespace oac_tree_gui
{

AggregateFactory::AggregateFactory()
{
  Register("if-then-else", Examples::CreateIfThenElseAggregate);
}

std::unique_ptr<InstructionItem> AggregateFactory::Create(const std::string &type_name)
{
  return GetValue(type_name)();  // call factory function
}

}  // namespace oac_tree_gui
