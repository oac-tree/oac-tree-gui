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

#include "oac_tree_gui/composer/widget_focus_handler.h"

#include <oac_tree_gui/model/procedure_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief The MockEditor class to test ProcedureComposerFocusManager.
 */
class MockEditor
{
public:
  MOCK_METHOD(void, SetProcedure, (ProcedureItem*));
  MOCK_METHOD(void, ShowAsActive, (bool));
  MOCK_METHOD(void, ShowAsLastEditor, (bool));
};

}  // namespace

/**
 * @brief Tests of WidgetFocusHandler class.
 */
class WidgetFocusHandlerTest : public ::testing::Test
{
public:
  using mock_editor_t = ::testing::StrictMock<MockEditor>;
};

TEST_F(WidgetFocusHandlerTest, InitialState)
{
  const WidgetFocusHandler<MockEditor> focus_manager;

  EXPECT_EQ(focus_manager.GetCount(), 0U);
  EXPECT_EQ(focus_manager.GetInFocus(), nullptr);
}

TEST_F(WidgetFocusHandlerTest, AddWidgetAndRemove)
{
  WidgetFocusHandler<MockEditor> focus_manager;
  mock_editor_t editor1;

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(true)).Times(1);
  }

  focus_manager.AddWidget(&editor1);
  EXPECT_EQ(focus_manager.GetCount(), 1U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor1);

  focus_manager.RemoveWidget(&editor1);
  EXPECT_EQ(focus_manager.GetCount(), 0U);
  EXPECT_EQ(focus_manager.GetInFocus(), nullptr);
}

TEST_F(WidgetFocusHandlerTest, AddWidgetInBetween)
{
  WidgetFocusHandler<MockEditor> focus_manager;
  mock_editor_t editor1;
  mock_editor_t editor2;
  mock_editor_t editor3;

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(true)).Times(1);
  }

  focus_manager.AddWidget(&editor1);
  EXPECT_EQ(focus_manager.GetCount(), 1U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor1);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(false)).Times(1);
    EXPECT_CALL(editor2, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(false)).Times(1);
    EXPECT_CALL(editor2, ShowAsLastEditor(::testing::_)).Times(0);
  }

  focus_manager.AddWidget(&editor2, &editor1);
  EXPECT_EQ(focus_manager.GetCount(), 2U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor2);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(::testing::_)).Times(0);
    EXPECT_CALL(editor2, ShowAsActive(false)).Times(1);
    EXPECT_CALL(editor3, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(::testing::_)).Times(0);
    EXPECT_CALL(editor2, ShowAsLastEditor(::testing::_)).Times(0);
    EXPECT_CALL(editor3, ShowAsLastEditor(::testing::_)).Times(0);
  }
  focus_manager.AddWidget(&editor3, &editor1);
  EXPECT_EQ(focus_manager.GetCount(), 3U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor3);

  EXPECT_EQ(focus_manager.GetWidgets()[0], &editor1);
  EXPECT_EQ(focus_manager.GetWidgets()[1], &editor3);
  EXPECT_EQ(focus_manager.GetWidgets()[2], &editor2);
}

TEST_F(WidgetFocusHandlerTest, AddTwoWidgetsAndRemove)
{
  WidgetFocusHandler<MockEditor> focus_manager;
  mock_editor_t editor1;
  mock_editor_t editor2;

  // adding first editor
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(true)).Times(1);
  }

  focus_manager.AddWidget(&editor1);
  EXPECT_EQ(focus_manager.GetCount(), 1U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor1);

  // adding second time is no-op
  focus_manager.AddWidget(&editor1);
  EXPECT_EQ(focus_manager.GetCount(), 1U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor1);

  // adding the second editor
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(false)).Times(1);
    EXPECT_CALL(editor2, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(false)).Times(1);
    EXPECT_CALL(editor2, ShowAsLastEditor(::testing::_)).Times(0);
  }

  focus_manager.AddWidget(&editor2);
  EXPECT_EQ(focus_manager.GetCount(), 2U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor2);

  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(true)).Times(1);
    EXPECT_CALL(editor2, ShowAsActive(::testing::_)).Times(0);
    EXPECT_CALL(editor2, ShowAsLastEditor(::testing::_)).Times(0);
  }

  focus_manager.RemoveWidget(&editor2);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor1);

  // removing second time is no-op
  focus_manager.RemoveWidget(&editor2);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor1);
}

TEST_F(WidgetFocusHandlerTest, AddThreeWidgetsAndChangeFocus)
{
  WidgetFocusHandler<MockEditor> focus_manager;
  mock_editor_t editor1;
  mock_editor_t editor2;
  mock_editor_t editor3;

  // adding first editor
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(true)).Times(1);
  }

  focus_manager.AddWidget(&editor1);
  EXPECT_EQ(focus_manager.GetCount(), 1U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor1);

  // adding the second editor
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(false)).Times(1);
    EXPECT_CALL(editor2, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(false)).Times(1);
    EXPECT_CALL(editor2, ShowAsLastEditor(::testing::_)).Times(0);
  }

  focus_manager.AddWidget(&editor2);
  EXPECT_EQ(focus_manager.GetCount(), 2U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor2);

  // adding third editor
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(::testing::_)).Times(0);
    EXPECT_CALL(editor2, ShowAsActive(false)).Times(1);
    EXPECT_CALL(editor3, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor1, ShowAsLastEditor(::testing::_)).Times(0);
    EXPECT_CALL(editor2, ShowAsLastEditor(::testing::_)).Times(0);
    EXPECT_CALL(editor3, ShowAsLastEditor(::testing::_)).Times(0);
  }

  focus_manager.AddWidget(&editor3);
  EXPECT_EQ(focus_manager.GetCount(), 3U);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor3);

  // set middle widget in focus
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(::testing::_)).Times(0);
    EXPECT_CALL(editor1, ShowAsLastEditor(::testing::_)).Times(0);
    EXPECT_CALL(editor3, ShowAsActive(false)).Times(1);
    EXPECT_CALL(editor2, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor2, ShowAsLastEditor(::testing::_)).Times(0);
    EXPECT_CALL(editor3, ShowAsLastEditor(::testing::_)).Times(0);
  }
  focus_manager.SetInFocus(&editor2);

  // setting same focus is no-op (no extra callbacks)
  focus_manager.SetInFocus(&editor2);

  // removing second widget
  {
    const ::testing::InSequence seq;
    EXPECT_CALL(editor1, ShowAsActive(::testing::_)).Times(0);
    EXPECT_CALL(editor1, ShowAsLastEditor(::testing::_)).Times(0);
    EXPECT_CALL(editor2, ShowAsActive(::testing::_)).Times(0);
    EXPECT_CALL(editor2, ShowAsLastEditor(::testing::_)).Times(0);
    EXPECT_CALL(editor3, ShowAsActive(true)).Times(1);
    EXPECT_CALL(editor3, ShowAsLastEditor(::testing::_)).Times(0);
  }
  focus_manager.RemoveWidget(&editor2);
  EXPECT_EQ(focus_manager.GetInFocus(), &editor3);
}

}  // namespace oac_tree_gui
