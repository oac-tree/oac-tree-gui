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

#ifndef OAC_TREE_GUI_COMPONENTS_MIME_CONSTANTS_H_
#define OAC_TREE_GUI_COMPONENTS_MIME_CONSTANTS_H_

//! @file
//! Collection of constants for mime objects.

namespace oac_tree_gui
{

//! Mime type for drag-and-drop events containing item's identifier information.
constexpr auto kItemIdentifierMimeType = "application/coa.sequencer.item.identifier";
constexpr auto kInstructionIdentifierMimeType = "application/coa.sequencer.instruction.identifier";
constexpr auto kJobIdentifierMimeType = "application/coa.sequencer.job.identifier";
constexpr auto kProcedureIdentifierMimeType = "application/coa.sequencer.procedure.identifier";

//! Mime type for drag-and-drop events leading to the creation of new instruction.
constexpr auto kNewInstructionMimeType = "application/coa.sequencer.instruction.new";

//! Mime type for procedure copy.
constexpr auto kProcedureCopyMimeType = "application/coa.sequencer.procedure.copy";

//! mime type to hold instruction copy
constexpr auto kInstructionCopyMimeType = "application/coa.sequencer.instruction.copy";

//! Mime type for variable copy.
constexpr auto kVariableCopyMimeType = "application/coa.sequencer.variable.copy";

//! mime type to hold top selected item types
constexpr auto kTopSelectedItemTypesMimeType = "application/coa.sequencer.topselecteditemtypes";

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPONENTS_DRAG_AND_DROP_HELPER_H_
