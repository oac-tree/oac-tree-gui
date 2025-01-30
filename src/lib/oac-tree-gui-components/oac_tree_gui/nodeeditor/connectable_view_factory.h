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

#ifndef OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_FACTORY_H_
#define OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_FACTORY_H_

#include <oac_tree_gui/nodeeditor/view_factory_interface.h>

#include <memory>

namespace oac_tree_gui
{

/**
 * @brief The ConnectableViewFactory class is a factory to construct ConnectableViews from
 * corresponding items.
 */
class ConnectableViewFactory : public ViewFactoryInterface
{
public:
  ConnectableViewFactory();
  ~ConnectableViewFactory() override;

  std::unique_ptr<ConnectableView> CreateView(InstructionItem *item) override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_FACTORY_H_
