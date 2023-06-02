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

#include "aggregate_factory.h"

#include <sequencergui/model/aggregate_examples.h>
#include <sequencergui/model/instruction_item.h>

namespace sequencergui
{

AggregateFactory::AggregateFactory()
{
  Register("if-then-else", Examples::CreateIfThenElseAggregate);
}

std::unique_ptr<InstructionItem> AggregateFactory::Create(const std::string &type_name)
{
  return GetValue(type_name)();  // call factory function
}

}  // namespace sequencergui
