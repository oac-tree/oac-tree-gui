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

#ifndef LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
#define LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_

//! @file
//! Collection of utils to test sequencer.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/model/instruction_status.h>
#include <oac_tree_gui/model/runner_status.h>

#include <mvvm/model/tagindex.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace mvvm
{
class ISessionModel;
class SessionItem;
}

namespace oac_tree_gui
{

class VariableItem;
class InstructionItem;
class IJobHandler;
class JobItem;

namespace test
{

using attribute_list_t = std::vector<std::pair<std::string, std::string>>;

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
sup::oac_tree::JobInfo CreateJobInfo(const std::string& procedure_text);

/**
 * @brief Checks if the value caried inside VariableItem is equal to a given AnyValue.
 */
bool IsEqual(const oac_tree_gui::VariableItem& variable, const sup::dto::AnyValue& value);

/**
 * @brief Creates a variable item of the given domain type and inserts it into the parent.
 *
 * @param domain_type The domain variable type.
 * @param parent The parent to get a new child.
 * @param tag_index Child position.
 * @return Inserted item.
 */
VariableItem* InsertVariable(const std::string& domain_type, mvvm::SessionItem* parent,
                             const mvvm::TagIndex& tag_index = mvvm::TagIndex::Append());

/**
 * @brief Finds InstructionItem representing given domain type.
 */
std::vector<const InstructionItem*> FindInstructions(const mvvm::ISessionModel& model,
                                                     const std::string& domain_type);
std::vector<InstructionItem*> FindInstructions(mvvm::ISessionModel& model,
                                               const std::string& domain_type);

/**
 * @brief Creates VariableItem from the VariableInfo of a domain variable, as for jobs.
 */
std::unique_ptr<VariableItem> CreateJobVariableItem(const std::string& domain_type,
                                                    sup::dto::uint32 index = 0U,
                                                    const attribute_list_t& attributes = {});

/**
 * @brief Processes Qt events until the job is not running and its JobItem has the expected status.
 *
 * For a local job, all earlier instruction, variable and log events have reached the model too.
 */
::testing::AssertionResult WaitForCompletion(IJobHandler& job_handler,
                                             RunnerStatus expected = RunnerStatus::kSucceeded,
                                             std::chrono::milliseconds timeout = kWaitTimeout);

/**
 * @brief Processes Qt events until the domain job is running.
 */
::testing::AssertionResult WaitForRunning(const IJobHandler& job_handler,
                                          std::chrono::milliseconds timeout = kWaitTimeout);

/**
 * @brief Processes Qt events until the JobItem has the expected status.
 */
::testing::AssertionResult WaitForJobStatus(const JobItem& job_item, RunnerStatus expected,
                                            std::chrono::milliseconds timeout = kWaitTimeout);

/**
 * @brief Processes Qt events until the instruction has the expected status.
 */
::testing::AssertionResult WaitForInstructionStatus(
    const InstructionItem& instruction, InstructionStatus expected,
    std::chrono::milliseconds timeout = kWaitTimeout);

/**
 * @brief Processes Qt events until the variable holds the expected value.
 */
::testing::AssertionResult WaitForValue(const VariableItem& variable,
                                        const sup::dto::AnyValue& expected,
                                        std::chrono::milliseconds timeout = kWaitTimeout);

}  // namespace test

}  // namespace oac_tree_gui

#endif  // LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
