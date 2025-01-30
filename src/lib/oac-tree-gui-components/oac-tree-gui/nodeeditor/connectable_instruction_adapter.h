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

#ifndef OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_INSTRUCTION_ADAPTER_H_
#define OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_INSTRUCTION_ADAPTER_H_

#include "port_info.h"

#include <vector>

class QColor;
class QString;

namespace oac_tree_gui
{

class InstructionItem;

/**
 * @brief The ConnectableInstructionAdapter is a convenience class that adapts InstructionItem for
 * InstructionView.
 *
 * Introduced to isolate a view from InstructionItem, and also supplement InstructionItem with
 * additional methods specific to a graphics scene.
 */
class ConnectableInstructionAdapter
{
public:
  explicit ConnectableInstructionAdapter(InstructionItem* instruction);

  QColor GetColor() const;

  QString GetDisplayName() const;

  /**
   * @brief Returns input ports of connectable view.
   *
   * This port is located on an instruction playing the role of a child. Usually located on the top
   * part of the view. The connection from this port should go toward parent view.
   */
  virtual std::vector<PortInfo> GetInputPorts() const;

  /**
   * @brief Returns output ports of connectable view.
   *
   * This port is located on an instruction playing the role of a parent. Usually located on the
   * bottom part of the view. The connections from this port should go toward children.
   */
  virtual std::vector<PortInfo> GetOutputPorts() const;

  InstructionItem* GetInstruction() const;

  double GetX() const;

  void SetX(double value);

  double GetY() const;

  void SetY(double value);

  bool IsRoot() const;

  void SetXY(double x, double y);

private:
  InstructionItem* m_instruction{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_NODEEDITOR_CONNECTABLE_INSTRUCTION_ADAPTER_H_
