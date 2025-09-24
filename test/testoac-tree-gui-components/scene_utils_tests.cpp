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

#include "oac_tree_gui/nodeeditor/scene_utils.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/utils/numeric_utils.h>

#include <sup/oac-tree/instruction.h>
#include <sup/oac-tree/instruction_registry.h>

#include <gtest/gtest.h>

#include <QPointF>
#include <QRectF>

namespace oac_tree_gui::test
{

namespace
{
bool operator==(const std::vector<QPointF>& lhs, const std::vector<QPointF>& rhs)
{
  using mvvm::utils::AreAlmostEqual;
  if (lhs.size() != rhs.size())
  {
    return false;
  }
  for (std::size_t i = 0; i < lhs.size(); ++i)
  {
    if (lhs[i] != rhs[i])
    {
      return false;
    }
  }
  return true;
}

}  // namespace

//! Tests for items from instructionitems.h

class SceneUtilsTest : public ::testing::Test
{
public:
  using points_t = std::vector<QPointF>;
};

TEST_F(SceneUtilsTest, VectorOfPointsEquality)
{
  using points_t = std::vector<QPointF>;
  EXPECT_TRUE(points_t{} == points_t{});
  EXPECT_FALSE(points_t{QPointF(1.0, 2.0)} == points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.0)}));
  EXPECT_TRUE(points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.0)})
              == points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.0)}));
  EXPECT_FALSE(points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.0)})
               == points_t({QPointF(1.0, 2.0), QPointF(1.0, 2.01)}));
}

TEST_F(SceneUtilsTest, InsertSpaceAtCamelCase)
{
  EXPECT_EQ(InsertSpaceAtCamelCase(""), std::string(""));
  EXPECT_EQ(InsertSpaceAtCamelCase(" "), std::string(" "));
  EXPECT_EQ(InsertSpaceAtCamelCase("abc"), std::string("abc"));
  EXPECT_EQ(InsertSpaceAtCamelCase("abc def"), std::string("abc def"));
  EXPECT_EQ(InsertSpaceAtCamelCase("Access"), std::string("Access"));
  EXPECT_EQ(InsertSpaceAtCamelCase("PVAccess"), std::string("PVAccess"));

  EXPECT_EQ(InsertSpaceAtCamelCase("ChannelAccess"), std::string("Channel Access"));
  EXPECT_EQ(InsertSpaceAtCamelCase("PvAccess"), std::string("Pv Access"));
  EXPECT_EQ(InsertSpaceAtCamelCase("NoCamelCase"), std::string("No Camel Case"));
  EXPECT_EQ(InsertSpaceAtCamelCase("ChannelAccessRead"), std::string("Channel Access Read"));
}

TEST_F(SceneUtilsTest, GetNodeDropPosition)
{
  const auto bounding_box = ConnectableViewRectangle();

  const QPointF top_left_corner{0.0, 0.0};
  EXPECT_EQ(GetNodeDropPosition(bounding_box.center()), top_left_corner);
}

}  // namespace oac_tree_gui::test
