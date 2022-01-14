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

#include "sequencergui/model/importutils.h"

#include "Procedure.h"
#include "SequenceParser.h"
#include "sequencergui/model/sequenceritems.h"
#include "sequencergui/model/transformfromdomain.h"

#include <iostream>
#include <stdexcept>
namespace sequencergui
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

}  // namespace sequencergui
