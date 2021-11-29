/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_IMPORTUTILS_H
#define SEQUENCERGUI_MODEL_IMPORTUTILS_H

//! @file importutils.h
//! Collection of utility functions to import xml files from disk into SessionModel.

#include <memory>
#include <string>

namespace sequi
{
class ProcedureItem;

//! Imports content of xml file with Sequencer Procedure into ProcedureItem.
//! ProcedureItem must be empty.
void ImportFromFile(const std::string& file_name, ProcedureItem* procedure_item);

}  // namespace sequi

#endif // SEQUENCERGUI_MODEL_IMPORTUTILS_H
