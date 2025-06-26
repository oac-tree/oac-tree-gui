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

#ifndef LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
#define LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_

//! @file
//! Collection of utils to test sequencer.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <sup/gui/core/dto_types_fwd.h>

#include <memory>
#include <string>
#include <vector>

namespace mvvm
{
class ISessionModel;
}

namespace oac_tree_gui
{

class VariableItem;
class VariableInfoItem;
class InstructionItem;

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
 * @brief Finds InstructionItem representing given domain type.
 */
std::vector<InstructionItem*> FindInstructions(const mvvm::ISessionModel& model,
                                               const std::string& domain_type);

/**
 * @brief Creates VariableItem from string representing the type of sup::oac_tree::Variable.
 */
std::unique_ptr<VariableInfoItem> CreateVariableInfoItem(const std::string& domain_type,
                                                         sup::dto::uint32 index = 0U,
                                                         const attribute_list_t& attributes = {});

}  // namespace test

}  // namespace oac_tree_gui

#endif  // LIBTEST_UTILS_TESTUTILS_EPICS_TEST_UTILS_H_
