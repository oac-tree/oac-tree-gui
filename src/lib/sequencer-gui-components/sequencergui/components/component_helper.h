/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef SEQUENCERGUI_COMPONENTS_COMPONENT_HELPER_H_
#define SEQUENCERGUI_COMPONENTS_COMPONENT_HELPER_H_

//! @file
//! Collection of helper function to retrieve values of presentation-related constants.

#include <sequencergui/components/component_types.h>

#include <QColor>
#include <QFont>

namespace sequencergui
{

namespace constants
{
const QString kValidateProcedureCommandId = "Validate procedure";
const QString kExportXmlCommandId = "Export XML";
const QString kToggleLeftSideBar = "Show/hide left side bar";
const QString kToggleRightSideBar = "Show/hide right side bar";
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_COMPONENT_HELPER_H_
