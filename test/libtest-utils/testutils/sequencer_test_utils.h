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

#ifndef LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
#define LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_

//! Collection of utils to test sequencer.

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sup/gui/core/dto_types_fwd.h>

#include <memory>
#include <string>
#include <vector>

namespace testutils
{

//! Returns string representing EPICS database file with several testing variables.
std::string GetEpicsDBContentString();

//! Creates sequencer local variable with the given name and initial value.
std::unique_ptr<variable_t> CreateLocalVariable(const std::string& name,
                                                const sup::dto::AnyValue& initial_value);

//! Creates PvAccessServerVariable with given parameters
std::unique_ptr<variable_t> CreatePVAccessServerVariable(const std::string& name,
                                                         const std::string& channel_name,
                                                         const sup::dto::AnyValue& anyvalue);

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
