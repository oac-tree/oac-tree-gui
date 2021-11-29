/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_DOMAINOBJECTBUILDER_H
#define SEQUENCERGUI_MODEL_DOMAINOBJECTBUILDER_H

#include "sequencergui/model/sequencer_types.h"

#include <map>
#include <memory>
#include <string>

namespace sequi
{
class ProcedureItem;
class InstructionItem;
class InstructionContainerItem;
class WorkspaceItem;

//! Creates domain Procedure from ProcedureItem.
//! Saves correspondence of SessionItem identifiers to newly created domain objects.

class DomainObjectBuilder
{
public:
  [[nodiscard]] std::unique_ptr<procedure_t> CreateProcedure(const ProcedureItem* procedure_item);
  ~DomainObjectBuilder();

  void BuildProcedure(const ProcedureItem* procedure_item);

  procedure_t* GetProcedure() const;

  std::string FindInstructionIdentifier(const instruction_t* instruction) const;

  std::string FindVariableItemIdentifier(const variable_t* variable) const;
  std::string FindVariableItemIdentifier(const std::string& variable_name) const;

private:
  void Iterate(const sequi::InstructionItem* instruction, instruction_t* parent);
  void PopulateDomainInstructions(const InstructionContainerItem* container, procedure_t* procedure);
  void PopulateDomainWorkspace(const WorkspaceItem* workspace, procedure_t* procedure);

  std::unique_ptr<procedure_t> m_procedure;        //!< domain procedure
  const ProcedureItem* m_procedure_item{nullptr};  //!< item used to build domain procedure
  std::map<const instruction_t*, std::string> m_instruction_to_id;
  std::map<const variable_t*, std::string> m_variable_to_id;
  std::map<std::string, std::string> m_variablename_to_id;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_DOMAINOBJECTBUILDER_H
