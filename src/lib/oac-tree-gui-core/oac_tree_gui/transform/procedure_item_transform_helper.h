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

#ifndef OAC_TREE_GUI_TRANSFORM_PROCEDURE_ITEM_TRANSFORM_HELPER_H_
#define OAC_TREE_GUI_TRANSFORM_PROCEDURE_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of utility functions to convert sup::oac_tree::Procedure into SessionModel.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <memory>

namespace oac_tree_gui
{

class ProcedureItem;
class ProcedurePreambleItem;

/**
 * @brief Populates empty PreambleItem with the domain information.
 */
void PopulateProcedurePreambleItem(const preamble_t& preamble, ProcedurePreambleItem& item);

/**
 * @brief Creates ProcedureItem from given domain procedure.
 *
 * Its main purpose is to generate a GUI object from the result of XML import. It is expected, that
 * the domain procedure's Setup() hasn't been performed yet.
 */
std::unique_ptr<ProcedureItem> CreateProcedureItem(const procedure_t& procedure);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_PROCEDURE_ITEM_TRANSFORM_HELPER_H_
