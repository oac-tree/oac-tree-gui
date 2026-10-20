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

#include "oac_tree_gui/transform/instruction_attribute_presets.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>

#include <sup/gui/model/scalar_conversion_utils.h>

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>
#include <sup/oac-tree/attribute_definition.h>
#include <sup/oac-tree/instruction.h>

#include <gtest/gtest.h>

#include <algorithm>

namespace oac_tree_gui::test
{

//! Tests for functions from instruction_attribute_presets.h.
class InstructionAttributePresetsTest : public ::testing::Test
{
};

TEST_F(InstructionAttributePresetsTest, FindAttributePreset)
{
  auto preset = FindAttributePreset(domainconstants::kRepeatInstructionType,
                                    domainconstants::kMaxCountAttribute);
  ASSERT_TRUE(preset.has_value());
  EXPECT_EQ(preset->value, mvvm::variant_t{mvvm::int32{-1}});

  // collapsed Include is the default of IsCollapsed, not a preset
  EXPECT_FALSE(FindAttributePreset(domainconstants::kIncludeInstructionType,
                                   domainconstants::kShowCollapsedAttribute)
                   .has_value());
  EXPECT_FALSE(FindAttributePreset(domainconstants::kSequenceInstructionType,
                                   domainconstants::kShowCollapsedAttribute)
                   .has_value());
  EXPECT_FALSE(
      FindAttributePreset(domainconstants::kWaitInstructionType, domainconstants::kTimeoutAttribute)
          .has_value());
  EXPECT_FALSE(
      FindAttributePreset(domainconstants::kRepeatInstructionType, "non-existing").has_value());
}

//! Each preset matches the type of the domain attribute definition.
TEST_F(InstructionAttributePresetsTest, PresetTypesMatchDomainDefinitions)
{
  for (const auto& [key, preset] : GetAttributePresets())
  {
    const auto pos = key.find('/');
    ASSERT_NE(pos, std::string::npos) << key;
    const auto attribute_name = key.substr(pos + 1);

    auto instruction = CreateDomainInstruction(key.substr(0, pos));
    const auto& definitions = instruction->GetAttributeDefinitions();
    auto iter = std::find_if(definitions.begin(), definitions.end(),
                             [&attribute_name](const auto& definition)
                             { return definition.GetName() == attribute_name; });
    ASSERT_NE(iter, definitions.end()) << key;

    EXPECT_EQ(iter->GetType(), sup::gui::GetAnyValueFromScalar(preset.value).GetType()) << key;
  }
}

}  // namespace oac_tree_gui::test
