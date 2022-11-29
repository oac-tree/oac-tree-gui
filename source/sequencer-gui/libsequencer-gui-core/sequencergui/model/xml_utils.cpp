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

#include "sequencergui/model/xml_utils.h"

#include <sequencergui/model/domain_procedure_builder.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/transform_from_domain.h>
#include <sup/sequencer/procedure.h>
#include <sup/sequencer/sequence_parser.h>
#include <sequencergui/model/domain_workspace_builder.h>

#include <iostream>
#include <stdexcept>
namespace sequencergui
{
std::unique_ptr<ProcedureItem> ImportFromFile(const std::string &file_name)
{
  auto result = std::make_unique<ProcedureItem>();
  auto procedure = sup::sequencer::ParseProcedureFile(file_name);

  if (!procedure)
  {
    throw std::runtime_error("Error: uninitialised procedure");
  }

  PopulateProcedureItem(procedure.get(), result.get(), /*root_only*/ false);

  return result;
}

std::string ExportToXMLString(const ProcedureItem *procedure_item)
{
  DomainProcedureBuilder builder;
  auto domain_procedure = builder.CreateProcedure(procedure_item);
  return GetXMLString(*domain_procedure);
}

}  // namespace sequencergui
