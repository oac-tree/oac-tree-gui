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

#ifndef SEQUENCERGUI_MODEL_DOMAIN_PROCEDURE_BUILDER_H_
#define SEQUENCERGUI_MODEL_DOMAIN_PROCEDURE_BUILDER_H_

#include <sequencergui/model/sequencer_types.h>

#include <map>
#include <memory>
#include <string>

namespace sequencergui
{
class ProcedureItem;
class InstructionItem;
class InstructionContainerItem;
class WorkspaceItem;

//! Creates domain Procedure from ProcedureItem.
//! Saves correspondence of SessionItem identifiers to newly created domain objects.

class DomainProcedureBuilder
{
public:
  [[nodiscard]] std::unique_ptr<procedure_t> CreateProcedure(const ProcedureItem* procedure_item);
  ~DomainProcedureBuilder();

  void BuildProcedure(const ProcedureItem* procedure_item);

  procedure_t* GetProcedure() const;

  std::string FindInstructionIdentifier(const instruction_t* instruction) const;

  std::string FindVariableItemIdentifier(const variable_t* variable) const;
  std::string FindVariableItemIdentifier(const std::string& variable_name) const;

private:
  void Iterate(const sequencergui::InstructionItem* instruction, instruction_t* parent);
  void PopulateDomainInstructions(const InstructionContainerItem* container,
                                  procedure_t* procedure);
  void PopulateDomainWorkspace(const WorkspaceItem* workspace, procedure_t* procedure);

  std::unique_ptr<procedure_t> m_procedure;        //!< domain procedure
  const ProcedureItem* m_procedure_item{nullptr};  //!< item used to build domain procedure
  std::map<const instruction_t*, std::string> m_instruction_to_id;
  std::map<const variable_t*, std::string> m_variable_to_id;
  std::map<std::string, std::string> m_variablename_to_id;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_DOMAIN_PROCEDURE_BUILDER_H_
