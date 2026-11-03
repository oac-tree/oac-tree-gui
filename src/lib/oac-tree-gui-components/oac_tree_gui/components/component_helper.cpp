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

#include "component_helper.h"

#include <oac_tree_gui/model/variable_item.h>

#include <sup/gui/model/anyvalue_item.h>

namespace oac_tree_gui
{

QColor GetConnectedVariableColor()
{
  static const QColor result("#00aa00");  // green
  return result;
}

QColor GetDisonnectedVariableColor()
{
  static const QColor result("#b9b9b9");  // gray
  return result;
}

QColor GetReadOnlyDomainTypeColor()
{
  return QColor(Qt::gray);
}

sup::gui::AnyValueItem* GetInlineValueItem(VariableItem& item)
{
  auto* anyvalue_item = item.GetAnyValueItem();
  if (anyvalue_item == nullptr)
  {
    return nullptr;
  }

  if (anyvalue_item->IsScalar())
  {
    return anyvalue_item;
  }

  if (anyvalue_item->IsStruct() && (anyvalue_item->GetChildrenCount() == 1U))
  {
    auto* field_item = anyvalue_item->GetChildren().at(0);
    return field_item->IsScalar() ? field_item : nullptr;
  }

  return nullptr;
}

}  // namespace oac_tree_gui
