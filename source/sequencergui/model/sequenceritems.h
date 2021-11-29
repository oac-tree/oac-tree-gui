/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_SEQUENCERITEMS_H
#define SEQUENCERGUI_MODEL_SEQUENCERITEMS_H

//! @file sequenceritems.h
//! Collection of classes defining main components of SequencerModel.

#include "sequencergui/model/instructionitems.h"
#include "sequencergui/model/workspaceitems.h"

#include "mvvm/standarditems/standarditemincludes.h"
#include "mvvm/model/compounditem.h"

#include <string>

namespace sequi
{
//! Represents unknown instruction.
class UnknownInstructionItem : public InstructionItem
{
public:
  static inline const std::string Type = "UnknownInstruction";
  UnknownInstructionItem();

  void InitFromDomain(const instruction_t* instruction);
};

//! Represents Instructions.
class InstructionContainerItem : public ModelView::CompoundItem
{
public:
  static inline const std::string Type = "InstructionContainer";
  InstructionContainerItem();

  std::vector<InstructionItem*> GetInstructions() const;
};

//! Represents Procedure.
class ProcedureItem : public ModelView::CompoundItem
{
public:
  static inline const std::string Type = "Procedure";
  ProcedureItem();

  InstructionContainerItem* GetInstructionContainer() const;
  WorkspaceItem* GetWorkspace() const;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_SEQUENCERITEMS_H
