/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef TEST_PROCEDURE_ITEMS_H
#define TEST_PROCEDURE_ITEMS_H

//! @file test_procedure_items.h
//! Collection of pocedure items for testing purpose.

#include <sequencergui/domain/sequencer_types.h>

#include <chrono>
#include <memory>

namespace sequencergui
{
class ProcedureItem;
class SequencerModel;
}  // namespace sequencergui

namespace testutils
{

//! Creates invalid procedure that will cause JobContext to crash during the preparation.
sequencergui::ProcedureItem* CreateInvalidProcedure(sequencergui::SequencerModel* model);

//! Creates procedure with single wait instruction.
sequencergui::ProcedureItem* CreateSingleWaitProcedure(sequencergui::SequencerModel* model,
                                                       std::chrono::milliseconds timeout);

//! Creates procedure with two variables and single Copy instruction.
sequencergui::ProcedureItem* CreateCopyProcedure(sequencergui::SequencerModel* model);

//! Creates procedure with local include.
sequencergui::ProcedureItem* CreateIncludeProcedure(sequencergui::SequencerModel* model);

//! Creates procedure that requires input from the user.
sequencergui::ProcedureItem* CreateInputProcedure(sequencergui::SequencerModel* model);

//! Creates procedure that requires user choice.
sequencergui::ProcedureItem* CreateUserChoiceProcedure(sequencergui::SequencerModel* model);

}  // namespace testutils

#endif  // TEST_PROCEDURE_ITEMS_H
