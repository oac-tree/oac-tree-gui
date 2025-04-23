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

#ifndef OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_MAP_H_
#define OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_MAP_H_

#include <map>
#include <vector>

namespace oac_tree_gui
{

class InstructionItem;
class ConnectableView;

//! Stores correspondance between InstructionItem's and ConnectableView's.
//! Plays supporting role for GraphicsSceneController

class ConnectableViewMap
{
public:
  ConnectableViewMap();

  void Insert(const InstructionItem* item, ConnectableView* view_item);

  void InsertOrAssign(const InstructionItem* item, ConnectableView* view_item);

  ConnectableView* FindView(const InstructionItem* item);

  void Remove(const InstructionItem* item);

  void Remove(ConnectableView* view);

  ConnectableView* TakeView(const InstructionItem* item);

  void Clear();

  std::vector<ConnectableView*> FindRelatedViews(const InstructionItem* item);

  int GetSize() const;

private:
  std::map<const InstructionItem*, ConnectableView*> m_item_to_view;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_VIEW_MAP_H_
