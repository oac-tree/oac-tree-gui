/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
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

  //! Creates domain instruction corresponding to given item.
  virtual std::unique_ptr<instruction_t> CreateDomainInstruction() const;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;

  std::string m_domain_name;
  std::vector<std::string> m_domain_attributes;
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
