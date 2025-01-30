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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_NODEEDITOR_VIEW_FACTORY_INTERFACE_H_
#define OAC_TREE_GUI_NODEEDITOR_VIEW_FACTORY_INTERFACE_H_

#include <memory>

namespace oac_tree_gui
{

class InstructionItem;
class ConnectableView;

class ViewFactoryInterface
{
public:
  virtual ~ViewFactoryInterface() = default;
  ViewFactoryInterface() = default;
  ViewFactoryInterface(const ViewFactoryInterface&) = delete;
  ViewFactoryInterface& operator=(const ViewFactoryInterface&) = delete;

  virtual std::unique_ptr<ConnectableView> CreateView(InstructionItem*) = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_VIEW_FACTORY_INTERFACE_H_
