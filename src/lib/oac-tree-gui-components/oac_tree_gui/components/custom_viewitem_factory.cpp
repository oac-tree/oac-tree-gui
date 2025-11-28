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

#include "custom_viewitem_factory.h"

#include "custom_presentation_items.h"

#include <oac_tree_gui/model/sequencer_item_helper.h>

#include <mvvm/model/session_item.h>
#include <mvvm/providers/viewitem.h>
#include <mvvm/providers/viewitem_factory.h>

#include <QString>

namespace oac_tree_gui
{

std::unique_ptr<mvvm::ViewItem> CreateChannelPresentationViewItem(mvvm::SessionItem& item)
{
  auto is_available_property = oac_tree_gui::GetIsAvailableItem(item);
  auto channel_property = oac_tree_gui::GetChannelItem(item);

  if ((is_available_property != nullptr) && (channel_property != nullptr))
  {
    auto channel_name = QString::fromStdString(channel_property->Data<std::string>());
    auto presentation = std::make_unique<oac_tree_gui::ChannelPresentationItem>(
        is_available_property, channel_name);
    return std::make_unique<mvvm::ViewItem>(std::move(presentation));
  }

  return mvvm::CreateLabelViewItem(&item);
}

}  // namespace oac_tree_gui
