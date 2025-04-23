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

#ifndef OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_
#define OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_

#include <mvvm/signals/event_types.h>

#include <memory>

namespace mvvm
{
class ISessionModel;
}

namespace oac_tree_gui
{

class NodeGraphicsScene;
class SequencerModel;
class InstructionContainerItem;

/**
 * @brief The ConnectableViewModelController class listens for changes in instruction container and
 * updates connectable nodes on the graphics scene.
 */
class ConnectableViewModelController
{
public:
  ConnectableViewModelController(mvvm::ISessionModel *model, NodeGraphicsScene *graphics_scene);
  ~ConnectableViewModelController();

  void OnModelEvent(const mvvm::ItemInsertedEvent &event);

  void OnModelEvent(const mvvm::AboutToRemoveItemEvent &event);

  void OnModelEvent(const mvvm::DataChangedEvent &event);

  void Init(InstructionContainerItem *root_item);

private:
  struct ConnectableViewModelControllerImpl;
  std::unique_ptr<ConnectableViewModelControllerImpl> p_impl;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_MODEL_CONTROLLER_H_
