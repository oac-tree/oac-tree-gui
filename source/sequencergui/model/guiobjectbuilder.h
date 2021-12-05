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

#ifndef SEQUENCERGUI_MODEL_GUIOBJECTBUILDER_H
#define SEQUENCERGUI_MODEL_GUIOBJECTBUILDER_H

#include "sequencergui/model/sequencer_types.h"

#include <map>
#include <string>

namespace ModelView
{
class SessionItem;
}

namespace sequi
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

class GUIObjectBuilder
{
public:
  void PopulateProcedureItem(const procedure_t* procedure, ProcedureItem* procedure_item);

  void PopulateInstructionContainerItem(const procedure_t* procedure,
                                        InstructionContainerItem* container,
                                        bool root_only = false);

  void PopulateWorkspaceItem(const procedure_t* procedure, WorkspaceItem* workspace);

  // FIXME remove unused methods
  std::string FindInstructionItemIdentifier(const instruction_t* instruction) const;
  InstructionItem* FindInstructionItem(const instruction_t* instruction) const;
  std::string FindVariableItemIdentifier(const variable_t* variable) const;
  VariableItem* FindVariableItem(const variable_t* variable) const;
  std::string FindVariableItemIdentifier(const std::string& variable_name) const;
  VariableItem* FindVariableItem(const std::string& variable_name) const;

private:
  ModelView::SessionItem* ProcessInstruction(const instruction_t* instruction,
                                             ModelView::SessionItem* parent);
  void Iterate(const instruction_t* instruction, ModelView::SessionItem* parent);

  void Save(const instruction_t* instruction, InstructionItem* item);
  void Save(const variable_t* variable, VariableItem* item);

  std::map<const instruction_t*, InstructionItem*> m_to_instruction_item;

  std::map<const variable_t*, VariableItem*> m_variable_to_id;
  std::map<std::string, VariableItem*> m_variablename_to_id;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_GUIOBJECTBUILDER_H
