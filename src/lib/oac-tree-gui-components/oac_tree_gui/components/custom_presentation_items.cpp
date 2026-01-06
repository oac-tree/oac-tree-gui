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

#include "custom_presentation_items.h"

#include <oac_tree_gui/components/component_helper.h>

#include <mvvm/model/session_item.h>

namespace oac_tree_gui
{

ChannelPresentationItem::ChannelPresentationItem(mvvm::SessionItem* item,
                                                 const QString& channel_name)
    : mvvm::DataPresentationItem(item, mvvm::DataRole::kDisplay), m_channel_name(channel_name)
{
}

QVariant ChannelPresentationItem::Data(mvvm::role_t qt_role) const
{
  static const QColor kConnectedColor = GetConnectedVariableColor();
  static const QColor kDisonnectedColor = GetDisonnectedVariableColor();

  if (qt_role == Qt::DecorationRole)
  {
    return GetItem()->Data<bool>() ? kConnectedColor : kDisonnectedColor;
  }

  if (qt_role == Qt::DisplayRole)
  {
    return m_channel_name;
  }

  return {};
}

bool ChannelPresentationItem::SetData(const QVariant& data, mvvm::role_t qt_role)
{
  (void)data;
  (void)qt_role;
  return false;
}

QVector<int> ChannelPresentationItem::GetQtRoles(mvvm::role_t data_role) const
{
  auto result = mvvm::DataPresentationItem::GetQtRoles(data_role);
  // When IsAvailable status changes, the decoration role (green/gray box rectangle) should be
  // reported too
  result.push_back(Qt::DecorationRole);
  return result;
}

}  // namespace oac_tree_gui
