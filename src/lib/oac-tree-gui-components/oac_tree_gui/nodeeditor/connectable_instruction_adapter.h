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

#ifndef OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_INSTRUCTION_ADAPTER_H_
#define OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_INSTRUCTION_ADAPTER_H_

#include <mvvm/nodeeditor/i_connectable_shape_adapter.h>

namespace oac_tree_gui
{

class InstructionItem;

/**
 * @brief The ConnectableInstructionAdapter class adapts InstructionItem to ConnectableShape.
 */
class ConnectableInstructionAdapter : public mvvm::IConnectableShapeAdapter
{
public:
  explicit ConnectableInstructionAdapter(InstructionItem* instruction);

  mvvm::SessionItem* GetItem() const override;

  QString GetDisplayName() const override;

  QColor GetBaseColor() const override;

  double GetX() const override;

  void SetX(double value) override;

  double GetY() const override;

  void SetY(double value) override;

  std::vector<mvvm::PortInfo> GetPortInfos() const override;

  InstructionItem* GetInstructionItem() const;

  mvvm::NodeOperationStates GetOperationStates() const override;

  void ProcessEvent(const mvvm::node_event_t& event) const override;

private:
  InstructionItem* m_instruction{nullptr};
};

}  // namespace oac_tree_gui

#endif // OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_INSTRUCTION_ADAPTER_H_

