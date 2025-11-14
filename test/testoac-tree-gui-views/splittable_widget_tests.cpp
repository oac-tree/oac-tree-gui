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

#include "oac_tree_gui/views/composer/splittable_widget.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSplitter>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for SplittableWidget class.
 */
class SplittableWidgetTest : public ::testing::Test
{
public:
  ::testing::MockFunction<std::function<std::unique_ptr<QWidget>()>> m_mock_widget;
};

TEST_F(SplittableWidgetTest, AddWidget)
{
  auto test_widget0 = std::make_unique<QWidget>();
  auto test_widget0_ptr = test_widget0.get();

  EXPECT_CALL(m_mock_widget, Call())
      .WillOnce(::testing::Return(::testing::ByMove(std::move(test_widget0))));

  SplittableWidget widget(m_mock_widget.AsStdFunction());
  EXPECT_NE(widget.GetSplitter(), nullptr);

  EXPECT_EQ(widget.GetSplitter()->count(), 1);
  EXPECT_EQ(widget.GetSplitter()->widget(0), test_widget0_ptr);

  // adding second widget
  auto test_widget1 = std::make_unique<QWidget>();
  auto test_widget1_ptr = test_widget1.get();
  EXPECT_CALL(m_mock_widget, Call())
      .WillOnce(::testing::Return(::testing::ByMove(std::move(test_widget1))));

  widget.AddWidget();
  EXPECT_EQ(widget.GetSplitter()->count(), 2);
  EXPECT_EQ(widget.GetSplitter()->widget(1), test_widget1_ptr);

  // adding third widget after the first
  auto test_widget2 = std::make_unique<QWidget>();
  auto test_widget2_ptr = test_widget2.get();
  EXPECT_CALL(m_mock_widget, Call())
      .WillOnce(::testing::Return(::testing::ByMove(std::move(test_widget2))));

  widget.AddWidget(test_widget0_ptr);  // adding after the first
  EXPECT_EQ(widget.GetSplitter()->count(), 3);
  EXPECT_EQ(widget.GetSplitter()->widget(0), test_widget0_ptr);
  EXPECT_EQ(widget.GetSplitter()->widget(1), test_widget2_ptr);
  EXPECT_EQ(widget.GetSplitter()->widget(2), test_widget1_ptr);
}

}  // namespace oac_tree_gui::test
