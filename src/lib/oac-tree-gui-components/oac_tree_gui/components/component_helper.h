/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_COMPONENTS_COMPONENT_HELPER_H_
#define OAC_TREE_GUI_COMPONENTS_COMPONENT_HELPER_H_

//! @file
//! Collection of presentation-related helper functions.

#include <QColor>

namespace sup::gui
{
class AnyValueItem;
}

namespace oac_tree_gui
{

class VariableItem;

namespace constants
{
constexpr auto kValidateProcedureCommandId = "Validate procedure";
constexpr auto kExportXmlCommandId = "Export XML";
}  // namespace constants

/**
 * @brief Returns color for the decoration used to display connected variables.
 */
QColor GetConnectedVariableColor();

/**
 * @brief Returns color for the decoration used to display connected variables.
 */
QColor GetDisonnectedVariableColor();

/**
 * @brief Returns color for the decoration used to display read-only types.
 */
QColor GetReadOnlyDomainTypeColor();

/**
 * @brief Returns the scalar shown in the value cell of the variable table row.
 *
 * If variable contains a single scalar AnyValueItem, the method will return this item.
 * If variable contains a struct with single scalar field, the method will return AnyValueItem
 * representing this field. In all other cases will return nullptr.
 *
 * The method is used to create variable table without showing simple underlying AnyValueItem
 * in a dedicated branch. Such scalar items will be shown in a "value" column next to variable name.
 *
 * @param item The variable.
 * @return The scalar item, or nullptr.
 */
sup::gui::AnyValueItem* GetInlineValueItem(VariableItem& item);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_COMPONENT_HELPER_H_
