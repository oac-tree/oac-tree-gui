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

#ifndef SEQUENCERGUI_MODEL_XML_UTILS_H_
#define SEQUENCERGUI_MODEL_XML_UTILS_H_

//! Collection of utility functions to import xml files from disk into SessionModel.

#include <memory>
#include <string>

namespace oac_tree_gui
{
class ProcedureItem;

/**
 * @brief Returns ProcedureItem representing a sequencer procedure stored in given xml file.
 */
std::unique_ptr<ProcedureItem> ImportFromFile(const std::string& file_name);

/**
 * @brief Exports procedure to XML string.
 * @param procedure_item
 * @return
 */
std::string ExportToXMLString(const ProcedureItem& procedure_item);

/**
 * @brief Replaces HTML quotation marks with double quotes, and turn existing double quotes to
 * single quotes.
 *
 * @param str Original string.
 * @return New string without quotation marks.
 *
 * @details
 * @code{xml}
 *    type="{&quot;type&quot;:&quot;uint64&quot;}"
 * @endcode
 * should become
 * @code{xml}
 *    type='{"type":"uint64"}'
 * @endcode
 *
 * @details If HTML quotation marks are not there, leave everything, including quotes, untouched.
 */

std::string ReplaceQuotationMarks(const std::string& str);
}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_MODEL_XML_UTILS_H_
