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

#ifndef LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURE_ITEMS_H_
#define LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURE_ITEMS_H_

//! @file
//! Collection of pocedure items for testing purpose.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <chrono>
#include <memory>

namespace oac_tree_gui
{

class ProcedureItem;
class SequencerModel;

namespace test
{

/**
 * @brief Creates invalid procedure that will cause JobHandler to crash during the preparation.
 */
std::unique_ptr<oac_tree_gui::ProcedureItem> CreateInvalidProcedureItem();

/**
 * @brief Creates invalid procedure that will cause JobHandler to crash during the preparation.
 *
 * The procedure is created in the procedure container of the given model.
 */
oac_tree_gui::ProcedureItem* CreateInvalidProcedureItem(oac_tree_gui::SequencerModel* model);

/**
 * @brief Creates procedure with single wait instruction.
 */
std::unique_ptr<oac_tree_gui::ProcedureItem> CreateSingleWaitProcedureItem(
    std::chrono::milliseconds timeout);

/**
 * @brief Creates procedure with single wait instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
oac_tree_gui::ProcedureItem* CreateSingleWaitProcedureItem(oac_tree_gui::SequencerModel* model,
                                                           std::chrono::milliseconds timeout);

/**
 * @brief Creates procedure with two variables and single Copy instruction.
 */
std::unique_ptr<oac_tree_gui::ProcedureItem> CreateCopyProcedureItem();

/**
 * @brief Creates procedure with two variables and single Copy instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
oac_tree_gui::ProcedureItem* CreateCopyProcedureItem(oac_tree_gui::SequencerModel* model);

/**
 * @brief Creates procedure with local include.
 */
std::unique_ptr<oac_tree_gui::ProcedureItem> CreateIncludeProcedureItem();

/**
 * @brief Creates procedure with local include.
 *
 * The procedure is created in the procedure container of the given model.
 */
oac_tree_gui::ProcedureItem* CreateIncludeProcedureItem(oac_tree_gui::SequencerModel* model);

/**
 * @brief Creates procedure that requires input from the user.
 */
std::unique_ptr<oac_tree_gui::ProcedureItem> CreateInputProcedureItem();

/**
 * @brief Creates procedure that requires input from the user.
 *
 * The procedure is created in the procedure container of the given model.
 */
oac_tree_gui::ProcedureItem* CreateInputProcedureItem(oac_tree_gui::SequencerModel* model);

/**
 * @brief Creates procedure that requires user choice.
 */
std::unique_ptr<oac_tree_gui::ProcedureItem> CreateUserChoiceProcedureItem();

/**
 * @brief Creates procedure that requires user choice.
 *
 * The procedure is created in the procedure container of the given model.
 */
oac_tree_gui::ProcedureItem* CreateUserChoiceProcedureItem(oac_tree_gui::SequencerModel* model);

/**
 * @brief Creates procedure with message instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
std::unique_ptr<oac_tree_gui::ProcedureItem> CreateMessageProcedureItem(const std::string& text);

/**
 * @brief Creates procedure with message instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
oac_tree_gui::ProcedureItem* CreateMessageProcedureItem(oac_tree_gui::SequencerModel* model,
                                                        const std::string& text);

/**
 * @brief Creates procedure demonstrating VariableReset instruction.
 */
std::unique_ptr<oac_tree_gui::ProcedureItem> CreateVariableResetProcedureItem();

/**
 * @brief Creates procedure demonstrating VariableReset instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
oac_tree_gui::ProcedureItem* CreateVariableResetProcedureItem(oac_tree_gui::SequencerModel* model);

/**
 * @brief Creates procedure demonstrating Increment instruction.
 *
 * Contains a sequence with two increment instructions, incrementing two different variables. Used
 */
std::unique_ptr<oac_tree_gui::ProcedureItem> CreateIncrementProcedureItem();

/**
 * @brief Creates procedure demonstrating Increment instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
oac_tree_gui::ProcedureItem* CreateIncrementProcedureItem(oac_tree_gui::SequencerModel* model);

}  // namespace test

}  // namespace oac_tree_gui

#endif  // LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURE_ITEMS_H_
