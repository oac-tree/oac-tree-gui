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
#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>

namespace anyvalueeditor
{

std::unique_ptr<AnyValueItem> CreateItem(const sup::dto::AnyValue &any_value)
{
  auto result = std::make_unique<AnyValueItem>();
  AnyValueItemBuilder m_builder(result.get());
  sup::dto::SerializeAnyValue(any_value, m_builder);
  return result;
}

}  // namespace anyvalueeditor
