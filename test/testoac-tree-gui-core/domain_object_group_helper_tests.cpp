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

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_object_type_registry.h>

#include <gtest/gtest.h>

#include "oac_tree_gui/domain/domain_object_group_helper.h"

namespace oac_tree_gui::test
{

//! Testing methods from composer_helper_tests.cpp
class DomainObjectGroupHelperTests : public ::testing::Test
{
};

TEST_F(DomainObjectGroupHelperTests, CreateInstructionTypeGroups)
{
  DomainObjectTypeRegistry registry;

  // We mimic sequential object registration during plugin load.
  std::vector<std::string> objects({"core1", "core2"});
  registry.Update(domainconstants::kCorePluginName, objects);

  objects.emplace_back("epics-ca1");
  registry.Update(domainconstants::kEpicsCAPluginName, objects);
  objects.emplace_back("epics-pvxs1");
  registry.Update(domainconstants::kEpicsPVXSPluginName, objects);

  objects.emplace_back("control1");
  registry.Update(domainconstants::kControlPluginName, objects);

  objects.emplace_back("config1");
  registry.Update(domainconstants::kSupConfigPluginName, objects);

  objects.emplace_back("timing1");
  registry.Update(domainconstants::kSupTimingPluginName, objects);

  objects.emplace_back("pulse-counter1");
  registry.Update(domainconstants::kSupPulseCounterPluginName, objects);

  objects.emplace_back("math1");
  objects.emplace_back("math2");
  registry.Update(domainconstants::kMathExprPluginName, objects);

  // validating name registration according to groups
  auto group_info = CreateInstructionTypeGroups(registry);

  ASSERT_EQ(group_info.size(), 7);
  EXPECT_EQ(group_info.at(0).group_name, kCoreGroup);
  EXPECT_EQ(group_info.at(0).object_names, std::vector<std::string>({"core1", "core2"}));

  EXPECT_EQ(group_info.at(1).group_name, kMathGroup);
  EXPECT_EQ(group_info.at(1).object_names, std::vector<std::string>({"math1", "math2"}));

  EXPECT_EQ(group_info.at(2).group_name, kEPICSGroup);
  EXPECT_EQ(group_info.at(2).object_names, std::vector<std::string>({"epics-ca1", "epics-pvxs1"}));

  EXPECT_EQ(group_info.at(3).group_name, kControlGroup);
  EXPECT_EQ(group_info.at(3).object_names, std::vector<std::string>({"control1"}));

  EXPECT_EQ(group_info.at(4).group_name, kTimingGroup);
  EXPECT_EQ(group_info.at(4).object_names, std::vector<std::string>({"timing1"}));

  EXPECT_EQ(group_info.at(5).group_name, kConfigGroup);
  EXPECT_EQ(group_info.at(5).object_names, std::vector<std::string>({"config1"}));

  EXPECT_EQ(group_info.at(6).group_name, kMiscGroup);
  EXPECT_EQ(group_info.at(6).object_names, std::vector<std::string>({"pulse-counter1"}));
}

}  // namespace oac_tree_gui::test
