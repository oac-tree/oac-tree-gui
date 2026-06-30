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

#ifndef OAC_TREE_GUI_MODEL_SERVER_INFO_ITEMS_ITEMS_H_
#define OAC_TREE_GUI_MODEL_SERVER_INFO_ITEMS_ITEMS_H_

//! @file
//! Collection of items to store automation server information.

#include <mvvm/model/compound_item.h>
#include <mvvm/standarditems/group_item.h>

namespace oac_tree_gui
{

class EPICSServerInfoItem : public mvvm::CompoundItem
{
public:
  EPICSServerInfoItem();

  std::unique_ptr<SessionItem> Clone() const override;
};

class WebSocketsServerInfoItem : public mvvm::CompoundItem
{
public:
  WebSocketsServerInfoItem();

  std::unique_ptr<SessionItem> Clone() const override;
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::EPICSServerInfoItem>
{
  static constexpr std::string_view type_name() noexcept { return "EPICSServerInfoItem"; }
};

template <>
struct item_traits<oac_tree_gui::WebSocketsServerInfoItem>
{
  static constexpr std::string_view type_name() noexcept { return "WebSocketsServerInfoItem"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_SERVER_INFO_ITEMS_ITEMS_H_