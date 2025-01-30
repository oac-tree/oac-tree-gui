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

#include "style_helper.h"

#include <sup/gui/style/style_helper.h>

#include <QIcon>

namespace oac_tree_gui
{

QIcon FindIcon(const QString &icon_name, sup::gui::IconColorFlavor icon_flavor)
{
  // in accordance with the oac_tree_icons.qrc file
  return sup::gui::utils::GetIcon(QString(":/oac-tree/icons/%1.svg").arg(icon_name), icon_flavor);
}

}  // namespace oac_tree_gui
