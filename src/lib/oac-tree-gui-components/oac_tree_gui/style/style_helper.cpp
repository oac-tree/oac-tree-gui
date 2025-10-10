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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "style_helper.h"

#include <oac_tree_gui/core/exceptions.h>

#include <sup/gui/style/style_helper.h>

#include <QFile>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>

namespace oac_tree_gui
{

QIcon FindIcon(const QString &icon_name, sup::gui::IconColorFlavor icon_flavor)
{
  // in accordance with the oac_tree_icons.qrc file
  return sup::gui::utils::GetIcon(QString(":/oac-tree/icons/%1.svg").arg(icon_name), icon_flavor);
}

QJsonObject LoadJsonFromResource(const QString &name)
{
  QFile file(name);

  if (!file.open(QIODevice::ReadOnly))
  {
    return {};
  }

  return QJsonDocument::fromJson(file.readAll()).object();
}

void ValidateStyleKey(const QJsonObject &json, const QString &group,
                      const QStringList &expected_keys)
{
  if (!json.contains(group))
  {
    throw RuntimeException(QString("Style does not contain '%1' group").arg(group).toStdString());
  }

  QJsonObject obj = json[group].toObject();

  for (const auto &key : expected_keys)
  {
    if (!obj.contains(key))
    {
      throw RuntimeException(QString("Style '%1' group does not contain expected '%2' key")
                                 .arg(group, key)
                                 .toStdString());
    }
  }
}

}  // namespace oac_tree_gui
