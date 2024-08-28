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

#ifndef LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
#define LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_

//! Collection of utils to test sequencer.

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sup/gui/core/dto_types_fwd.h>

#include <memory>
#include <string>

namespace testutils
{

/**
 * @brief Returns string representing EPICS database file with several testing variables.
 */
std::string GetEpicsDBContentString();

/**
 * @brief Creates sequencer local variable.
 */

/**
 * @brief Creates sequencer local variable with given parameters.
 */
std::unique_ptr<variable_t> CreateLocalVariable(const std::string& name,
                                                const sup::dto::AnyValue& initial_value);

/**
 * @brief Creates PvAccessClientVariable with given parameters.
 */
std::unique_ptr<variable_t> CreatePVAccessClientVariable(const std::string& name,
                                                         const sup::dto::AnyValue& initial_value,
                                                         const std::string& channel_name);

/**
 * @brief Creates PvAccessClientVariable with given parameters.
 */
std::unique_ptr<variable_t> CreatePVAccessServerVariable(const std::string& name,
                                                         const sup::dto::AnyValue& initial_value,
                                                         const std::string& channel_name);

/**
 * @brief Creates ChannelAccessVariable with given parameters.
 */
std::unique_ptr<variable_t> CreateChannelAccessVariable(const std::string& name,
                                                        const sup::dto::AnyValue& initial_value,
                                                        const std::string& channel_name);

/**
 * @brief Helper method to create JobInfo from procedure text body.
 */
sup::auto_server::JobInfo CreateJobInfo(const std::string& procedure_text);

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
