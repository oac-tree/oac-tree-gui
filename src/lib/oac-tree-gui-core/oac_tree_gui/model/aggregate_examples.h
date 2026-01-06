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

#ifndef OAC_TREE_GUI_MODEL_AGGREGATE_EXAMPLES_H_
#define OAC_TREE_GUI_MODEL_AGGREGATE_EXAMPLES_H_

//! Collection of factory functions to create complex instructions.

#include <memory>

namespace oac_tree_gui
{
class InstructionItem;

namespace Examples
{

//! Creates an aggregate representing  if-then-else statement
std::unique_ptr<InstructionItem> CreateIfThenElseAggregate();

}  // namespace Examples
}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_AGGREGATE_EXAMPLES_H_
