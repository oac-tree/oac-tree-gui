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

#ifndef SEQUENCERGUI_TRANSFORM_GUI_OBJECT_BUILDER_H_
#define SEQUENCERGUI_TRANSFORM_GUI_OBJECT_BUILDER_H_

#include <sup/gui/core/dto_types_fwd.h>

#include <sequencergui/transform/i_procedure_item_builder.h>
#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sup/sequencer/instruction_map.h>  // REFACTORING

#include <map>
#include <memory>
#include <string>

namespace mvvm
{
class SessionItem;
}

namespace sup::sequencer
{
class InstructionMap;
}

namespace sequencergui
{
class ProcedureItem;
class InstructionContainerItem;
class WorkspaceItem;
class InstructionItem;
class VariableItem;

//! Builds GUI objects from the domain procedure. It is intended to populate empty ProcedureItem
//! using the content of domain Procedure, and then use the resulting ProcedureItem to track
//! progress during domain execution.
//! - Uses only root instruction of domain's Procedure
//! - Procedure must be already set up (Procedure::Setup method called) to have all internal clones
//! already performed.

class ProcedureItemBuilder : public IProcedureItemBuilder
{
public:
  ~ProcedureItemBuilder() override;
  std::unique_ptr<ProcedureItem> CreateProcedureItem(const procedure_t* procedure, bool root_only);

  void PopulateProcedureItem(const procedure_t* procedure, ProcedureItem* procedure_item,
                             bool root_only);

  void PopulateInstructionContainerItem(const procedure_t* procedure,
                                        InstructionContainerItem* container, bool root_only);


  InstructionItem* GetInstruction(size_t domain_index) const override;

  InstructionItem* FindInstructionItem(const instruction_t* instruction) const;

  const instruction_t* FindInstruction(const InstructionItem* instruction_item) const;

  size_t GetIndex(const InstructionItem* instruction_item) const override;

  VariableItem* GetVariable(size_t index) const override;

private:
  mvvm::SessionItem* ProcessInstruction(const instruction_t* instruction,
                                        mvvm::SessionItem* parent);
  void Iterate(const instruction_t* instruction, mvvm::SessionItem* parent);

  void Save(const instruction_t* instruction, InstructionItem* item);
  void Save(const variable_t* variable, VariableItem* item);

  std::map<const instruction_t*, InstructionItem*> m_to_instruction_item;

  std::unique_ptr<sup::sequencer::InstructionMap> m_instruction_map; // REFACTORING
  std::vector<const sup::sequencer::Instruction*> m_index_to_instruction; // REFACTORING
  std::vector<const VariableItem*> m_index_to_variable;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_GUI_OBJECT_BUILDER_H_
