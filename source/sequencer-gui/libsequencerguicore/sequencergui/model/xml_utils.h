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

#ifndef SEQUENCERGUI_MODEL_XML_UTILS_H_
#define SEQUENCERGUI_MODEL_XML_UTILS_H_

//! @file importutils.h
//! Collection of utility functions to import xml files from disk into SessionModel.

#include <memory>
#include <string>

namespace sequencergui
{
class ProcedureItem;

//! Imports content of xml file with Sequencer Procedure into ProcedureItem.
//! ProcedureItem must be empty.
void ImportFromFile(const std::string& file_name, ProcedureItem* procedure_item);

//! Exports procedure to XML string.
std::string ExportToXMLString(const ProcedureItem* procedure_item);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_XML_UTILS_H_
