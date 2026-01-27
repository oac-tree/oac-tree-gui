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

#include "oac_tree_gui/domain/domain_object_group_helper.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/domain/domain_object_type_registry.h>

#include <mvvm/utils/container_utils.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

//! Testing methods from composer_helper_tests.cpp
class DomainObjectGroupHelperTests : public ::testing::Test
{
};

TEST_F(DomainObjectGroupHelperTests, GetDefaultPluginNameGroups)
{
  const std::vector<std::string> expected_groups{
      kCoreGroup, kMathGroup, kEPICSGroup, kControlGroup, kSupGroup, kMiscGroup,
  };
  EXPECT_EQ(GetDefaultPluginNameGroups(), expected_groups);
}

TEST_F(DomainObjectGroupHelperTests, CreatePluginNameGroups)
{
  std::vector<ObjectGroupInfo> group_info = CreatePluginNameGroups();

  std::vector<std::string> expected_groups = {kCoreGroup,    kMathGroup, kEPICSGroup,
                                              kControlGroup, kSupGroup,  kMiscGroup};
  ASSERT_EQ(group_info.size(), expected_groups.size());
  for (size_t i = 0; i < expected_groups.size(); ++i)
  {
    EXPECT_EQ(group_info.at(i).group_name, expected_groups.at(i));
  }

  // validating that group names contains plugin names and corresponding legacy names

  // kCoreGroup
  EXPECT_EQ(group_info.at(0).object_names,
            std::vector<std::string>(
                {domainconstants::kCorePluginName, domainconstants::kCorePluginName2}));
  // kMathGroup
  EXPECT_EQ(group_info.at(1).object_names,
            std::vector<std::string>({domainconstants::kMathExprPluginName, "sequencer-mathexpr"}));
  // kEPICSGroup
  EXPECT_EQ(group_info.at(2).object_names,
            std::vector<std::string>({domainconstants::kEpicsCAPluginName, "sequencer-ca",
                                      domainconstants::kEpicsPVXSPluginName, "sequencer-pvxs"}));
  // kControlGroup
  EXPECT_EQ(group_info.at(3).object_names,
            std::vector<std::string>({domainconstants::kControlPluginName, "sequencer-control"}));
  // kSupGroup
  EXPECT_EQ(
      group_info.at(4).object_names,
      std::vector<std::string>({domainconstants::kSupConfigPluginName, "sequencer-sup-config",
                                domainconstants::kSupTimingPluginName, "sequencer-sup-timing"}));

  // kMiscGroup
  EXPECT_EQ(group_info.at(5).object_names, std::vector<std::string>({
                                               domainconstants::kSystemPluginName,
                                               "sequencer-system",
                                               domainconstants::kStringPluginName,
                                               "sequencer-strings",
                                               domainconstants::kSupPulseCounterPluginName,
                                               "sequencer-sup-pulse-counter",
                                           }));
}

TEST_F(DomainObjectGroupHelperTests, GetGroupNameFromPluginName)
{
  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kCorePluginName), kCoreGroup);
  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kCorePluginName2), kCoreGroup);

  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kMathExprPluginName), kMathGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("sequencer-mathexpr"), kMathGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("liboac-tree-mathexpr.so"), kMathGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("libsequencer-mathexpr.so"), kMathGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("/opt/codac/lib/oac-tree/libsequencer-mathexpr.so"),
            kMathGroup);

  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kEpicsCAPluginName), kEPICSGroup);
  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kEpicsPVXSPluginName), kEPICSGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("sequencer-ca"), kEPICSGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("sequencer-pvxs"), kEPICSGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("libsequencer-ca.so"), kEPICSGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("libsequencer-pvxs.so"), kEPICSGroup);

  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kControlPluginName), kControlGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("sequencer-control"), kControlGroup);

  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kSupConfigPluginName), kSupGroup);
  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kSupTimingPluginName), kSupGroup);

  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kSystemPluginName), kMiscGroup);
  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kStringPluginName), kMiscGroup);
  EXPECT_EQ(GetGroupNameFromPluginName(domainconstants::kSupPulseCounterPluginName), kMiscGroup);
  EXPECT_EQ(GetGroupNameFromPluginName("unknown-plugin-name"), kMiscGroup);
}

TEST_F(DomainObjectGroupHelperTests, CreateInstructionTypeGroups)
{
  auto group_info = CreateInstructionTypeGroups();

  EXPECT_EQ(group_info.size(), GetDefaultPluginNameGroups().size());
  EXPECT_EQ(group_info.at(0).group_name, kCoreGroup);
  EXPECT_TRUE(mvvm::utils::Contains(group_info.at(0).object_names, "Choice"));

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  EXPECT_EQ(group_info.at(2).group_name, kEPICSGroup);
  EXPECT_TRUE(mvvm::utils::Contains(group_info.at(2).object_names,
                                    domainconstants::kPvAccessReadInstructionType));
}

}  // namespace oac_tree_gui::test
