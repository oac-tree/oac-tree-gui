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

#include <mvvm/model/sessionitem.h>

#include <QColor>

namespace
{
const QColor kConnectedColor("#00aa00");
const QColor kDisonnectedColor("#b9b9b9");
}  // namespace

namespace sequencergui
{

AvailablePresentationItem::AvailablePresentationItem(mvvm::SessionItem *item)
    : mvvm::DataPresentationItem(item, mvvm::DataRole::kDisplay)
{
}

QVariant AvailablePresentationItem::Data(int qt_role) const
{
  if (qt_role == Qt::DecorationRole)
  {
    return GetItem()->Data<bool>() ? kConnectedColor : kDisonnectedColor;
  }

  return {};
}

}  // namespace sequencergui
