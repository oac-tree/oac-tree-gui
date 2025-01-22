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

#ifndef LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURE_ITEMS_H_
#define LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURE_ITEMS_H_

//! @file
//! Collection of pocedure items for testing purpose.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <chrono>
#include <memory>

namespace sequencergui
{

class ProcedureItem;
class SequencerModel;

namespace test
{

/**
 * @brief Creates invalid procedure that will cause JobHandler to crash during the preparation.
 */
std::unique_ptr<sequencergui::ProcedureItem> CreateInvalidProcedureItem();

/**
 * @brief Creates invalid procedure that will cause JobHandler to crash during the preparation.
 *
 * The procedure is created in the procedure container of the given model.
 */
sequencergui::ProcedureItem* CreateInvalidProcedureItem(sequencergui::SequencerModel* model);

/**
 * @brief Creates procedure with single wait instruction.
 */
std::unique_ptr<sequencergui::ProcedureItem> CreateSingleWaitProcedureItem(
    std::chrono::milliseconds timeout);

/**
 * @brief Creates procedure with single wait instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
sequencergui::ProcedureItem* CreateSingleWaitProcedureItem(sequencergui::SequencerModel* model,
                                                           std::chrono::milliseconds timeout);

/**
 * @brief Creates procedure with two variables and single Copy instruction.
 */
std::unique_ptr<sequencergui::ProcedureItem> CreateCopyProcedureItem();

/**
 * @brief Creates procedure with two variables and single Copy instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
sequencergui::ProcedureItem* CreateCopyProcedureItem(sequencergui::SequencerModel* model);

/**
 * @brief Creates procedure with local include.
 */
std::unique_ptr<sequencergui::ProcedureItem> CreateIncludeProcedureItem();

/**
 * @brief Creates procedure with local include.
 *
 * The procedure is created in the procedure container of the given model.
 */
sequencergui::ProcedureItem* CreateIncludeProcedureItem(sequencergui::SequencerModel* model);

/**
 * @brief Creates procedure that requires input from the user.
 */
std::unique_ptr<sequencergui::ProcedureItem> CreateInputProcedureItem();

/**
 * @brief Creates procedure that requires input from the user.
 *
 * The procedure is created in the procedure container of the given model.
 */
sequencergui::ProcedureItem* CreateInputProcedureItem(sequencergui::SequencerModel* model);

/**
 * @brief Creates procedure that requires user choice.
 */
std::unique_ptr<sequencergui::ProcedureItem> CreateUserChoiceProcedureItem();

/**
 * @brief Creates procedure that requires user choice.
 *
 * The procedure is created in the procedure container of the given model.
 */
sequencergui::ProcedureItem* CreateUserChoiceProcedureItem(sequencergui::SequencerModel* model);

/**
 * @brief Creates procedure with message instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
std::unique_ptr<sequencergui::ProcedureItem> CreateMessageProcedureItem(const std::string& text);

/**
 * @brief Creates procedure with message instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
sequencergui::ProcedureItem* CreateMessageProcedureItem(sequencergui::SequencerModel* model,
                                                        const std::string& text);

/**
 * @brief Creates procedure demonstrating VariableReset instruction.
 */
std::unique_ptr<sequencergui::ProcedureItem> CreateVariableResetProcedureItem();

/**
 * @brief Creates procedure demonstrating VariableReset instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
sequencergui::ProcedureItem* CreateVariableResetProcedureItem(sequencergui::SequencerModel* model);

/**
 * @brief Creates procedure demonstrating Increment instruction.
 *
 * Contains a sequence with two increment instructions, incrementing two different variables. Used
 */
std::unique_ptr<sequencergui::ProcedureItem> CreateIncrementProcedureItem();

/**
 * @brief Creates procedure demonstrating Increment instruction.
 *
 * The procedure is created in the procedure container of the given model.
 */
sequencergui::ProcedureItem* CreateIncrementProcedureItem(sequencergui::SequencerModel* model);

}  // namespace test

}  // namespace sequencergui

#endif  // LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURE_ITEMS_H_
