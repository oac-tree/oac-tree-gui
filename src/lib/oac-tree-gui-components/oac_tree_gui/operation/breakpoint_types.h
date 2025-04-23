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

#ifndef OAC_TREE_GUI_OPERATION_BREAKPOINT_TYPES_H_
#define OAC_TREE_GUI_OPERATION_BREAKPOINT_TYPES_H_

#include <mvvm/model/path.h>

#include <cstdint>

namespace oac_tree_gui
{

enum class BreakpointStatus : std::uint8_t
{
  kNotSet,
  kSet,
  kDisabled
};

struct BreakpointInfo
{
  BreakpointStatus status;
  mvvm::Path path;  //! path to the instruction item
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_OPERATION_BREAKPOINT_TYPES_H_
