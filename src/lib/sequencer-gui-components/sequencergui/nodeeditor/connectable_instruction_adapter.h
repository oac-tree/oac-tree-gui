/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_CONNECTABLE_INSTRUCTION_ADAPTER_H_
#define SEQUENCERGUI_NODEEDITOR_CONNECTABLE_INSTRUCTION_ADAPTER_H_

#include "port_info.h"

#include <vector>

class QColor;
class QString;

namespace sequencergui
{
class InstructionItem;

//! Convenience class which adapts InstructionItem for InstructionView. Introduced to isolate
//! a view from InstructionItem, and also supplement InstructionItem with additional methods
//! specific for a graphics scene.

class ConnectableInstructionAdapter
{
public:
  explicit ConnectableInstructionAdapter(InstructionItem* instruction);

  QColor GetColor() const;

  QString GetDisplayName() const;

  virtual std::vector<PortInfo> GetInputPorts() const;

  virtual std::vector<PortInfo> GetOutputPorts() const;

  InstructionItem* GetInstruction() const;

  double GetX() const;

  void SetX(double value);

  double GetY() const;

  void SetY(double value);

  bool IsRoot() const;

private:
  InstructionItem* m_instruction{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_CONNECTABLE_INSTRUCTION_ADAPTER_H_
