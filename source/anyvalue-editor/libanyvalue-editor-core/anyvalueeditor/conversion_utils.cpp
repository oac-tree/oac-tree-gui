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

#include <anyvalueeditor/anyvalue_item.h>
#include <anyvalueeditor/anyvalue_item_builder.h>
#include <anyvalueeditor/conversion_utils.h>
#include <anyvalueeditor/domain_anyvalue_builder.h>
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

namespace anyvalueeditor
{

sup::dto::AnyValue CreateAnyValue(const AnyValueItem &item)
{
  DomainAnyValueBuilder builder(item);
  return builder.GetAnyValue();
}

std::unique_ptr<AnyValueItem> CreateItem(const sup::dto::AnyValue &any_value)
{
  AnyValueItemBuilder builder;
  sup::dto::SerializeAnyValue(any_value, builder);
  return std::move(builder.MoveAnyValueItem());
}

}  // namespace anyvalueeditor
