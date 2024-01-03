/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURES_H_
#define LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURES_H_

//! Collection of domain procedures for testing purpose.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <chrono>
#include <memory>

namespace testutils
{

//! Returns domain procedure that contains only a single wait instruction.
std::unique_ptr<procedure_t> CreateSingleWaitProcedure(std::chrono::milliseconds timeout);

//! Returns procedure that makes a single copy.
std::unique_ptr<procedure_t> CreateCopyProcedure();

//! Returns procedure with single message instruction.
std::unique_ptr<procedure_t> CreateMessageProcedure(const std::string& text);

//! Returns procedure with sequence and wait instruction in it.
std::unique_ptr<procedure_t> CreateSequenceWithWaitProcedure(std::chrono::milliseconds timeout);

//! Creates sequence with two waits.
std::unique_ptr<procedure_t> CreateSequenceWithTwoWaitsProcedure(
    std::chrono::milliseconds timeout1, std::chrono::milliseconds timeout2);

//! Creates sequence with single message.
std::unique_ptr<procedure_t> CreateSequenceWithSingleMessageProcedure();

//! Creates sequence with two messages.
std::unique_ptr<procedure_t> CreateSequenceWithTwoMessagesProcedure();

//! Creates procedure with one variable and input instruction.
std::unique_ptr<procedure_t> CreateInputProcedure();

//! Creates procedure with wait and copy instructions and possibility to select what to execute.
std::unique_ptr<procedure_t> CreateUserChoiceProcedure();

//! Returns domain procedure with a sequence inside a repeater.
std::unique_ptr<procedure_t> CreateRepeatSequenceProcedure(int count,
                                                           std::chrono::milliseconds timeout);

//! Returns a procedure with sequence included in repeater
//! <Sequence name="CountTwice">
//!     <Wait/>
//! </Sequence>
//! <Repeat isRoot="true" maxCount="10">
//!     <Include name="Counts" path="CountTwice"/>
//! </Repeat>
std::unique_ptr<procedure_t> CreateLocalIncludeProcedure();

//! Creates domain procedure with increment instruction repeated several times
std::unique_ptr<procedure_t> CreateCounterProcedure(int n_repetitions);

//! Creates domain procedure with repeat, sequence and possibility to terminate
std::unique_ptr<procedure_t> CreateRepeatSequencerProcedure(int n_repetitions, int max_counter);


}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_STANDARD_PROCEDURES_H_
