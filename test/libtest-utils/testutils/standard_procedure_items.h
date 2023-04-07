/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
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

//! Collection of pocedure items for testing purpose.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <chrono>
#include <memory>

namespace sequencergui
{
class ProcedureItem;
class SequencerModel;
}  // namespace sequencergui

namespace testutils
{

//! Creates invalid procedure that will cause JobHandler to crash during the preparation.
sequencergui::ProcedureItem* CreateInvalidProcedureItem(sequencergui::SequencerModel* model);

//! Creates procedure with single wait instruction.
sequencergui::ProcedureItem* CreateSingleWaitProcedureItem(sequencergui::SequencerModel* model,
                                                           std::chrono::milliseconds timeout);

//! Creates procedure with two variables and single Copy instruction.
sequencergui::ProcedureItem* CreateCopyProcedureItem(sequencergui::SequencerModel* model);

//! Creates procedure with local include.
sequencergui::ProcedureItem* CreateIncludeProcedureItem(sequencergui::SequencerModel* model);

//! Creates procedure that requires input from the user.
sequencergui::ProcedureItem* CreateInputProcedureItem(sequencergui::SequencerModel* model);

//! Creates procedure that requires user choice.
sequencergui::ProcedureItem* CreateUserChoiceProcedureItem(sequencergui::SequencerModel* model);

//! Creates procedure with message instruction.
sequencergui::ProcedureItem* CreateMessageProcedureItem(sequencergui::SequencerModel* model,
                                                        const std::string& text);

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURE_ITEMS_H_
