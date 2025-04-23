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
//! Collection of helper function to retrieve values of presentation-related constants.

#include <oac_tree_gui/components/component_types.h>

#include <QColor>
#include <QFont>

namespace oac_tree_gui
{

namespace constants
{
const QString kValidateProcedureCommandId = "Validate procedure";
const QString kExportXmlCommandId = "Export XML";
}  // namespace constants

/**
 * @brief Returns visual style of the behavior tree.
 */
BehaviorTreeStyle GetBehaviorTreeStyle();

/**
 * @brief Returns color for the decoration used to display connected variables.
 */
QColor GetConnectedVariableColor();

/**
 * @brief Returns color for the decoration used to display connected variables.
 */
QColor GetDisonnectedVariableColor();

/**
 * @brief Returns a string representing a custom tooltip style.
 */
QString GetCustomToolTipStyle();

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_COMPONENT_HELPER_H_
