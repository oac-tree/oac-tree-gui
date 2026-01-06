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

}  // namespace oac_tree_gui
