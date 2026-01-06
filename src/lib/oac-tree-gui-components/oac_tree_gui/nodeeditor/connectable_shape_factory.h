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

#ifndef OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_SHAPE_FACTORY_H_
#define OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_SHAPE_FACTORY_H_

#include <mvvm/nodeeditor/i_connectable_shape_factory.h>

namespace oac_tree_gui
{

class ConnectableShapeFactory : public mvvm::IConnectableShapeFactory
{
public:
  ConnectableShapeFactory() = default;
  ~ConnectableShapeFactory() override = default;

  ConnectableShapeFactory(const ConnectableShapeFactory&) = delete;
  ConnectableShapeFactory& operator=(const ConnectableShapeFactory&) = delete;
  ConnectableShapeFactory(ConnectableShapeFactory&&) = delete;
  ConnectableShapeFactory& operator=(ConnectableShapeFactory&&) = delete;

  std::unique_ptr<mvvm::ConnectableShape> CreateShape(mvvm::SessionItem* item) const override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_SHAPE_FACTORY_H_
