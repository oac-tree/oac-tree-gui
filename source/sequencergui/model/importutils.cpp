/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/model/importutils.h"

#include "Procedure.h"
#include "SequenceParser.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/transformfromdomain.h"

#include <iostream>
#include <stdexcept>
namespace sequi
{
void ImportFromFile(const std::string &file_name, ProcedureItem *procedure_item)
{
  auto procedure = sup::sequencer::ParseProcedureFile(file_name);
  if (!procedure)
  {
    throw std::runtime_error("Error: uninitialised procedure");
  }

  auto instruction_container = procedure_item->GetInstructionContainer();
  PopulateInstructionContainerItem(procedure.get(), instruction_container);

  auto workspace_item = procedure_item->GetWorkspace();
  PopulateWorkspaceItem(procedure.get(), workspace_item);
}

}  // namespace sequi
