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

#ifndef OAC_TREE_GUI_NODEEDITOR_I_CONNECTABLE_VIEW_FACTORY_H_
#define OAC_TREE_GUI_NODEEDITOR_I_CONNECTABLE_VIEW_FACTORY_H_

#include <memory>

namespace oac_tree_gui
{

class InstructionItem;
class ConnectableView;

/**
 * @brief The IConnectableViewFactory class is a base to construct connectable view nodes from
 * InstructionItem.
 */
class IConnectableViewFactory
{
public:
  IConnectableViewFactory() = default;
  virtual ~IConnectableViewFactory() = default;

  IConnectableViewFactory(const IConnectableViewFactory&) = delete;
  IConnectableViewFactory& operator=(const IConnectableViewFactory&) = delete;
  IConnectableViewFactory(IConnectableViewFactory&&) = delete;
  IConnectableViewFactory& operator=(IConnectableViewFactory&&) = delete;

  virtual std::unique_ptr<ConnectableView> CreateView(InstructionItem*) = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_I_CONNECTABLE_VIEW_FACTORY_H_
