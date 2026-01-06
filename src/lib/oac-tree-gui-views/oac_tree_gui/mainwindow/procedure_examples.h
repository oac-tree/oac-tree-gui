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

#ifndef OAC_TREE_GUI_MAINWINDOW_PROCEDURE_EXAMPLES_H_
#define OAC_TREE_GUI_MAINWINDOW_PROCEDURE_EXAMPLES_H_

//! Collection of example procedures to populate the model for debugging purposes.

namespace oac_tree_gui
{
class SequencerModel;
class ProcedureItem;

namespace Examples
{

//! Variable copying example.
ProcedureItem* AddCopyProcedure(SequencerModel* model);

//! Local include example.
ProcedureItem* AddLocalIncludeProcedure(SequencerModel* model);

//! User input example.
ProcedureItem* AddInputProcedure(SequencerModel* model);

//! User choice example.
ProcedureItem* AddUserChoiceProcedure(SequencerModel* model);

//! Example to test algorithm for complex alignment.
ProcedureItem* AddComplexAlignmentProcedure(SequencerModel* model);

//! Example to test algorithm for complex alignment.
ProcedureItem* AddComplexAlignmentProcedureV2(SequencerModel* model);

}  // namespace Examples
}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MAINWINDOW_PROCEDURE_EXAMPLES_H_
