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

#include "SequenceParser.h"
#include "sequencergui/model/domainobjectbuilder.h"
#include "sequencergui/model/importutils.h"
#include "sequencergui/model/sequenceritems.h"

#include <iostream>
#include <stdexcept>

namespace sequencergui
{
std::string ExportToXMLString(const ProcedureItem* procedure_item)
{
  DomainObjectBuilder builder;
  auto domain_procedure = builder.CreateProcedure(procedure_item);
  return GetXMLString(*domain_procedure);
}

}  // namespace sequencergui
