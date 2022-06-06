/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_CONNECTABLEVIEWMAP_H
#define SEQUENCERGUI_NODEEDITOR_CONNECTABLEVIEWMAP_H

#include <map>
#include <vector>

namespace sequencergui
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

private:
  std::map<const InstructionItem*, ConnectableView*> m_item_to_view;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_CONNECTABLEVIEWMAP_H
