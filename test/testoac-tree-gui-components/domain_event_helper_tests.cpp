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

#include <oac_tree_gui/jobsystem/domain_event_helper.h>

#include <sup/oac-tree/instruction_info.h>

#include <gtest/gtest.h>

#include <memory>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for methods from domain_event_helper.h
 */
class DomainEventHelperTest : public ::testing::Test
{
public:
  static std::unique_ptr<sup::oac_tree::InstructionInfo> CreateTestInstructionInfo()
  {
    auto result = std::make_unique<sup::oac_tree::InstructionInfo>(
        "sequence", sup::oac_tree::Instruction::kCompound, 0,
        std::vector<sup::oac_tree::AttributeInfo>());

    auto child0 = std::make_unique<sup::oac_tree::InstructionInfo>(
        "child0", sup::oac_tree::Instruction::kAction, 1,
        std::vector<sup::oac_tree::AttributeInfo>{});
    auto child0_ptr = child0.get();
    auto child1 = std::make_unique<sup::oac_tree::InstructionInfo>(
        "child1", sup::oac_tree::Instruction::kAction, 2,
        std::vector<sup::oac_tree::AttributeInfo>{});
    auto child1_ptr = child1.get();

    result->AppendChild(std::move(child0));
    result->AppendChild(std::move(child1));
    return result;
  }
};

TEST_F(DomainEventHelperTest, CreateInstructionAncestorFilter)
{
  const auto instruction_info = CreateTestInstructionInfo();
  auto filter = CreateInstructionAncestorFilter(*instruction_info);

  {
    const std::set<sup::dto::uint32> empty_set{};
    EXPECT_EQ(filter(empty_set), empty_set);
  }

  {
    const std::set<sup::dto::uint32> partial_set = {0, 1, 2};
    const std::set<sup::dto::uint32> expected_set = {0, 1, 2};
    EXPECT_EQ(filter(partial_set), expected_set);
  }

  {
    const std::set<sup::dto::uint32> partial_set = {1, 2};
    const std::set<sup::dto::uint32> expected_set = {};
    EXPECT_EQ(filter(partial_set), expected_set);
  }

  {
    const std::set<sup::dto::uint32> partial_set = {0, 1};
    const std::set<sup::dto::uint32> expected_set = {0, 1};
    EXPECT_EQ(filter(partial_set), expected_set);
  }
}

TEST_F(DomainEventHelperTest, CreateInstructionMuteAllFilter)
{
  auto filter = CreateInstructionMuteAllFilter();

  {
    const std::set<sup::dto::uint32> empty_set{};
    EXPECT_EQ(filter(empty_set), empty_set);
  }

  {
    const std::set<sup::dto::uint32> partial_set = {0, 1, 2};
    const std::set<sup::dto::uint32> expected_set = {};
    EXPECT_EQ(filter(partial_set), expected_set);
  }
}

TEST_F(DomainEventHelperTest, CreateInstructionIdentityFilter)
{
  auto filter = CreateInstructionIdentityFilter();

  {
    const std::set<sup::dto::uint32> empty_set{};
    EXPECT_EQ(filter(empty_set), empty_set);
  }

  {
    const std::set<sup::dto::uint32> partial_set = {0, 1, 2};
    const std::set<sup::dto::uint32> expected_set = {0, 1, 2};
    EXPECT_EQ(filter(partial_set), expected_set);
  }

  {
    const std::set<sup::dto::uint32> partial_set = {1, 2};
    const std::set<sup::dto::uint32> expected_set = {1, 2};
    EXPECT_EQ(filter(partial_set), expected_set);
  }

  {
    const std::set<sup::dto::uint32> partial_set = {0, 1};
    const std::set<sup::dto::uint32> expected_set = {0, 1};
    EXPECT_EQ(filter(partial_set), expected_set);
  }
}

}  // namespace oac_tree_gui::test
