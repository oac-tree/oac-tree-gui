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

#include "gui_domain_utils.h"

#include <sequencergui/model/standard_variable_items.h>
#include <sup/gui/dto/anyvalue_utils.h>

namespace testutils
{

void SetupVariable(const std::string &name, const sup::dto::AnyValue &initial_value,
                   sequencergui::LocalVariableItem &item)
{
  item.SetName(name);
  item.SetJsonType(sup::gui::GetAnyTypeToJSONString(&initial_value));
  item.SetJsonValue(sup::gui::GetValuesToJSONString(&initial_value));
}

}  // namespace testutils
