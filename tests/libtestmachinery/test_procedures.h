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

#ifndef TESTPROCEDURES_H
#define TESTPROCEDURES_H

//! @file test_procedures.h
//! Collection of domain rpocedures for testing purpose.

#include "sequencergui/model/sequencer_types.h"

#include <memory>

//! Various common utils for unit tests.

namespace testutils
{

//! Retuns domain procedure that contain only a single wait instruction.
std::unique_ptr<procedure_t> CreateSingleWaitProcedure(int msec_to_wait);

//! Returns procedure that makes a single copy.
std::unique_ptr<procedure_t> CreateCopyProcedure();

//! Creates sequence with two waits.
std::unique_ptr<procedure_t> CreateNestedProcedure();

//! Creates procedure with one variable and input instruction.
std::unique_ptr<procedure_t> CreateInputProcedure();

//! Creates procedure with wait and copy instructions and possibility to select what to execute.
std::unique_ptr<procedure_t> CreateUserChoiceProcedure();

}  // namespace testutils

#endif  // TESTPROCEDURES_H
