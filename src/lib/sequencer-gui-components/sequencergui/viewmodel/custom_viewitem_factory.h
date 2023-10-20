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

#ifndef SEQUENCERGUI_VIEWMODEL_CUSTOM_VIEWITEM_FACTORY_H
#define SEQUENCERGUI_VIEWMODEL_CUSTOM_VIEWITEM_FACTORY_H

//! @file
//! Collection of factory functions to create custom view items.

#include <memory>

namespace mvvm
{
class ViewItem;
class SessionItem;
}  // namespace mvvm

namespace sequencergui
{

/**
 * @brief Returns view item to present channel name and is_available status.
 *
 * @details If item doesn't have corresponding properties (channel name and is_available properties)
 * will return placeholder.
 */
std::unique_ptr<mvvm::ViewItem> CreateChannelPresentationViewItem(mvvm::SessionItem& item);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_CUSTOM_VIEWITEM_FACTORY_H
