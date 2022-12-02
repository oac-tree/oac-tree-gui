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

#include "sequencergui/transform/variable_item_transform_utils.h"

#include <sup/dto/anyvalue.h>
#include <sup/gui/dto/anyvalue_utils.h>
#include <sup/sequencer/variable.h>

namespace sequencergui
{

std::string GetValuesToJSONString(const variable_t *value)
{
  sup::dto::AnyValue anyvalue;
  value->GetValue(anyvalue);
  return sup::gui::GetValuesToJSONString(&anyvalue);
}

}  // namespace sequencergui
