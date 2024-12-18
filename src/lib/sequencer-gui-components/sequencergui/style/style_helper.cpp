/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

namespace sequencergui
{

// void RegisterPackageIcons()
// {
//   sup::gui::utils::RegisterPackageIcons();

//   const QString oac_tree_icons_resource = "oac_tree_icons.qrc";
//   const QString oac_tree_resource_path = ":/oac_tree/icons";  // as defined in file
//   sup::gui::utils::RegisterResource(oac_tree_icons_resource, oac_tree_resource_path,
//                                     sup::gui::utils::GetDefaultIconPathAlias());
// }

QIcon FindIcon(const QString &icon_name, sup::gui::IconColorFlavor icon_flavor)
{
  return sup::gui::utils::GetIcon(QString(":/oac-tree/icons/%1.svg").arg(icon_name), icon_flavor);
}

}  // namespace sequencergui
