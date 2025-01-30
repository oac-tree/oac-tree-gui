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

#ifndef SEQUENCERGUI_TRANSFORM_DOMAIN_PROCEDURE_BUILDER_H_
#define SEQUENCERGUI_TRANSFORM_DOMAIN_PROCEDURE_BUILDER_H_

#include <oac-tree-gui/domain/sequencer_types_fwd.h>

#include <map>
#include <memory>
#include <string>

namespace oac_tree_gui
{
class ProcedureItem;
class InstructionItem;
class InstructionContainerItem;
class WorkspaceItem;
class DomainWorkspaceBuilder;
class ProcedurePreambleItem;

//! Creates domain Procedure from ProcedureItem.
//! Saves correspondence of SessionItem identifiers to newly created domain objects.

class DomainProcedureBuilder
{
public:
  static std::unique_ptr<procedure_t> CreateProcedure(const ProcedureItem& procedure_item);

  ~DomainProcedureBuilder();

  void PopulateProcedure(const ProcedureItem& procedure_item, procedure_t& procedure);

  std::string FindInstructionIdentifier(const instruction_t* instruction) const;

  std::string FindVariableItemIdentifier(const variable_t* variable) const;

private:
  void Iterate(const oac_tree_gui::InstructionItem* instruction, instruction_t* parent);
  void PopulateDomainInstructions(const InstructionContainerItem* container,
                                  procedure_t* procedure);
  void PopulateDomainWorkspace(const WorkspaceItem* workspace, procedure_t* procedure);

  std::map<const instruction_t*, std::string> m_instruction_to_id;
  std::unique_ptr<DomainWorkspaceBuilder> m_workspace_builder;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_TRANSFORM_DOMAIN_PROCEDURE_BUILDER_H_
