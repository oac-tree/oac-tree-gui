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

#ifndef OAC_TREE_GUI_MODEL_SEQUENCER_SETTNG_CONSTANTS_H_
#define OAC_TREE_GUI_MODEL_SEQUENCER_SETTNG_CONSTANTS_H_

//! @file
//! Collection of constants related to the global application settings. These constants are used as
//! keys to register corresponding properties.

#include <cstdint>

namespace oac_tree_gui::constants
{

constexpr auto kUseUndoSetting = "kUseUndoSetting";
const bool kUseUndoDefault = true;
constexpr auto kUndoLimitSetting = "kUndoLimitSetting";
const std::int32_t kUndoLimitDefault = 100;

constexpr auto kUseTextEditor = "kUseTextEditor";
constexpr auto kTextEditorContent = "kTextEditorContent";

constexpr auto kPluginDirListProperty = "kPluginDirListProperty";
constexpr auto kPluginListProperty = "kPluginListProperty";

}  // namespace oac_tree_gui::constants

#endif  // OAC_TREE_GUI_MODEL_SEQUENCER_SETTNG_CONSTANTS_H_
