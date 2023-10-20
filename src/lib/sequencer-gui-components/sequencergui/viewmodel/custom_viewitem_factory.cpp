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

#include "custom_viewitem_factory.h"

#include "custom_presentation_items.h"

#include <sequencergui/model/sequencer_item_helper.h>

#include <mvvm/model/sessionitem.h>
#include <mvvm/viewmodel/viewitem_factory.h>
#include <mvvm/viewmodelbase/viewitem.h>

#include <QString>

namespace sequencergui
{

std::unique_ptr<mvvm::ViewItem> CreateChannelPresentationViewItem(mvvm::SessionItem &item)
{
  auto is_available_property = sequencergui::GetIsAvailableItem(item);
  auto channel_property = sequencergui::GetChannelItem(item);

  if (is_available_property && channel_property)
  {
    auto channel_name = QString::fromStdString(channel_property->Data<std::string>());
    auto presentation = std::make_unique<sequencergui::ChannelPresentationItem>(
        is_available_property, channel_name);
    return std::make_unique<mvvm::ViewItem>(std::move(presentation));
  }

  return mvvm::CreateLabelViewItem(&item);
}

}  // namespace sequencergui
