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

#ifndef OAC_TREE_GUI_OPERATION_TOOLTIP_HELPER_H_
#define OAC_TREE_GUI_OPERATION_TOOLTIP_HELPER_H_

//! @file
//! Collection of helper methods for tooltip generation.

#include <QString>
#include <vector>

namespace mvvm
{
class SessionItem;
}

namespace oac_tree_gui
{

/**
 * @brief Extracts item attributes that should go into tooltips of real-time instruction tree.
 */
std::vector<std::pair<std::string, std::string>> CollectToolTipAttributes(
    const mvvm::SessionItem* item);

/**
 * @brief Constructs html string containing a table with attribute <name, value> attributes.
 *
 * @param attributes The list of pairs name, and value for attribute as defined in InstructionInfo.
 * @param Total width of a tooltip in pixels.
 */
std::string GetAttributeHtml(const std::vector<std::pair<std::string, std::string>>& attributes,
                             int total_width);

/**
 * @brief Returns multi-line string representing the tooltip of instruction for real-time
 * instruction tree.
 */
QString GetInstructionToolTipText(const mvvm::SessionItem* item);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_OPERATION_TOOLTIP_HELPER_H_
