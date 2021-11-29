/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_CONNECTABLEINSTRUCTIONADAPTER_H
#define SEQUENCERGUI_NODEEDITOR_CONNECTABLEINSTRUCTIONADAPTER_H

#include "portinfo.h"

#include <vector>

class QColor;
class QString;

namespace sequi
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

private:
  InstructionItem* m_instruction{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_CONNECTABLEINSTRUCTIONADAPTER_H
