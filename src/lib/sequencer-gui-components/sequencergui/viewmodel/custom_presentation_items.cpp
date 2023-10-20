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

#include "custom_presentation_items.h"

#include <sequencergui/mainwindow/app_settings.h>

#include <mvvm/model/sessionitem.h>

namespace sequencergui
{

ChannelPresentationItem::ChannelPresentationItem(mvvm::SessionItem *item,
                                                 const QString &channel_name)
    : mvvm::DataPresentationItem(item, mvvm::DataRole::kDisplay), m_channel_name(channel_name)
{
}

QVariant ChannelPresentationItem::Data(int qt_role) const
{
  static const QColor kConnectedColor = GetConnectedColor();
  static const QColor kDisonnectedColor = GetDisonnectedColor();

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

}  // namespace sequencergui
