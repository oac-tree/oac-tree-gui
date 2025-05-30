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

#ifndef OAC_TREE_GUI_OPERATION_BREAKPOINT_HELPER_H_
#define OAC_TREE_GUI_OPERATION_BREAKPOINT_HELPER_H_

//! Collection of helper methods to handler breakpoints during instruction execution.

#include <oac_tree_gui/operation/breakpoint_types.h>

namespace oac_tree_gui
{

class InstructionItem;
class InstructionContainerItem;

/**
 * @brief Returns status of the breakpoint for given instruction.
 */
BreakpointStatus GetBreakpointStatus(const InstructionItem& item);

/**
 * @brief Sets status of the breakpoint for given instruction.
 */
void SetBreakpointStatus(const InstructionItem& item, BreakpointStatus status);

/**
 * @brief Toggles status of the breakpoint for given instruction.
 *
 * @details kNotSet -> kSet -> kDisabled -> kNotSet
 */
void ToggleBreakpointStatus(const InstructionItem& item);

/**
 * @brief Visits instruction tree and collect information about breakpoints.
 *
 * An instruction should belong to a model. Only report instructions with breakpoints either set, or
 * disabled.
 */
std::vector<BreakpointInfo> CollectBreakpointInfo(const InstructionItem& item);

/**
 * @brief Visits all instructions and collect information about breakpoints.
 *
 * Container should belong to a model. Only report instructions with breakpoints either set, or
 * disabled.
 */
std::vector<BreakpointInfo> CollectBreakpointInfo(const InstructionContainerItem& container);

/**
 * @brief Set breakpoint information to instruction item and underlying children.
 *
 * @details Method is used to populate the instruction hierarchy with breakpoint information taken
 * from another hierarchy. It is expected that the layout (parent/child relation) coincides for both
 * hierarchies.
 */
void SetBreakpointsFromInfo(const std::vector<BreakpointInfo>& info, InstructionItem& item);

/**
 * @brief Set breakpoint information to all instructions in the container.
 *
 * @details Method is used to populate the instruction hierarchy with breakpoint information taken
 * from another hierarchy. It is expected that the layout (parent/child relation) coincides for both
 * hierarchies.
 */
void SetBreakpointsFromInfo(const std::vector<BreakpointInfo>& info,
                            InstructionContainerItem& container);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_OPERATION_BREAKPOINT_HELPER_H_
