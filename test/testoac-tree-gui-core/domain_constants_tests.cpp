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

#include "oac_tree_gui/domain/domain_constants.h"

#include <sup/oac-tree/constants.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Validating that our constants coincide with the domain constants.
 */
class DomainConstantsTest : public ::testing::Test
{
};

//! Checking values of our constants wrt the constants defined in sup__oac_tree namespace.
//! We want to catch the moment, if oac-tree development change something.
TEST_F(DomainConstantsTest, CheckDomainConstants)
{
  EXPECT_EQ(domainconstants::kDescriptionAttribute,
            sup::oac_tree::Constants::DESCRIPTION_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kFailureThresholdAttribute,
            sup::oac_tree::Constants::FAILURE_THRESHOLD_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kFileNameAttribute, sup::oac_tree::Constants::FILENAME_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kInputVariableNameAttribute,
            sup::oac_tree::Constants::INPUT_VARIABLE_NAME_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kIsRootAttribute, sup::oac_tree::Constants::IS_ROOT_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kLeftVariableNameAttribute,
            sup::oac_tree::Constants::LEFT_VARIABLE_NAME_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kMaxCountAttribute,
            sup::oac_tree::Constants::MAX_COUNT_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kNameAttribute, sup::oac_tree::Constants::NAME_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kOutputVariableNameAttribute,
            sup::oac_tree::Constants::OUTPUT_VARIABLE_NAME_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kPathAttribute, sup::oac_tree::Constants::PATH_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kRightVariableNameAttribute,
            sup::oac_tree::Constants::RIGHT_VARIABLE_NAME_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kSuccessThresholdAttribute,
            sup::oac_tree::Constants::SUCCESS_THRESHOLD_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kTextAttribute, sup::oac_tree::Constants::TEXT_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kTimeoutAttribute,
            sup::oac_tree::Constants::TIMEOUT_SEC_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kTypeAttribute, sup::oac_tree::Constants::TYPE_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kValueAttribute, sup::oac_tree::Constants::VALUE_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kGenericVariableNameAttribute,
            sup::oac_tree::Constants::GENERIC_VARIABLE_NAME_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kSeverityAttribute, sup::oac_tree::Constants::SEVERITY_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kMessageAttribute, sup::oac_tree::Constants::MESSAGE_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kPrettyJsonAttribute,
            sup::oac_tree::Constants::PRETTY_JSON_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kDynamicTypeAttribute,
            sup::oac_tree::Constants::IS_DYNAMIC_TYPE_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kShowCollapsedAttribute,
            sup::oac_tree::Constants::SHOW_COLLAPSED_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kFromVariableAttributeName,
            sup::oac_tree::Constants::FROM_VARIABLE_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kEqualVariableAttributeName,
            sup::oac_tree::Constants::EQUALS_VARIABLE_NAME_ATTRIBUTE_NAME);
  EXPECT_EQ(domainconstants::kBlockingAttribute, sup::oac_tree::Constants::BLOCKING_ATTRIBUTE_NAME);
}

}  // namespace oac_tree_gui::test
