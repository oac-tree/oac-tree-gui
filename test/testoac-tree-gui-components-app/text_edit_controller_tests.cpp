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

#include "oac_tree_gui/components/text_edit_controller.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/text_edit_item.h>

#include <mvvm/model/application_model.h>
#include <mvvm/test/mock_item_listener.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QCheckBox>
#include <QSignalSpy>
#include <QTextEdit>

namespace oac_tree_gui::test
{

class TextEditControllerTest : public ::testing::Test
{
public:
  TextEditControllerTest() { m_text_edit_item = m_model.InsertItem<TextEditItem>(); }

  /**
   * @brief Creates a controller which is looking to properties of "plugins directory list".
   */
  std::unique_ptr<TextEditController> CreateController()
  {
    const TextControllerContext context{&m_check_box, &m_text_edit};
    return std::make_unique<TextEditController>(context);
    return {};
  }

  QCheckBox m_check_box;
  QTextEdit m_text_edit;
  mvvm::ApplicationModel m_model;
  TextEditItem* m_text_edit_item{nullptr};
  using mock_listener_t = ::testing::StrictMock<mvvm::test::MockItemListener>;
};

TEST_F(TextEditControllerTest, ConstructFailure)
{
  EXPECT_THROW(TextEditController({}), RuntimeException);
  EXPECT_THROW(TextEditController({nullptr, nullptr}), RuntimeException);
  EXPECT_THROW(TextEditController({&m_check_box, nullptr}), RuntimeException);
}

TEST_F(TextEditControllerTest, InitialStateWhenEmptyItem)
{
  auto controller = CreateController();
  controller->SetItem(m_text_edit_item);

  EXPECT_EQ(controller->GetItem(), m_text_edit_item);
  EXPECT_TRUE(m_check_box.isChecked());
  EXPECT_TRUE(m_text_edit.isEnabled());
  EXPECT_EQ(m_text_edit.toPlainText(), QString());
}

TEST_F(TextEditControllerTest, InitialStateWhenDisabledEditor)
{
  m_text_edit_item->SetEditorEnabled(false);
  m_text_edit_item->SetText({"line1", "line2"});

  auto controller = CreateController();
  controller->SetItem(m_text_edit_item);

  EXPECT_EQ(controller->GetItem(), m_text_edit_item);
  EXPECT_FALSE(m_check_box.isChecked());
  EXPECT_FALSE(m_text_edit.isEnabled());

  const QString expected_text = {R"RAW(line1
line2
)RAW"};
  EXPECT_EQ(m_text_edit.toPlainText(), expected_text);
}

TEST_F(TextEditControllerTest, UpdateWidgetOnItemChange)
{
  auto controller = CreateController();
  controller->SetItem(m_text_edit_item);
  EXPECT_TRUE(m_check_box.isChecked());
  EXPECT_TRUE(m_text_edit.isEnabled());
  EXPECT_EQ(m_text_edit.toPlainText(), QString());

  const QSignalSpy spy_check_box(&m_check_box, &QCheckBox::stateChanged);
  const QSignalSpy spy_text_edit(&m_text_edit, &QTextEdit::textChanged);

  m_text_edit_item->SetEditorEnabled(false);

  EXPECT_EQ(spy_check_box.count(), 1);
  EXPECT_EQ(spy_text_edit.count(), 1);

  EXPECT_FALSE(m_check_box.isChecked());
  EXPECT_FALSE(m_text_edit.isEnabled());
  m_text_edit_item->SetText({"line1", "line2"});
  const QString expected_text = {R"RAW(line1
line2
)RAW"};
  EXPECT_EQ(m_text_edit.toPlainText(), expected_text);
}

TEST_F(TextEditControllerTest, UpdateItemOnTextEditChange)
{
  auto controller = CreateController();
  controller->SetItem(m_text_edit_item);

  const QSignalSpy spy_check_box(&m_check_box, &QCheckBox::stateChanged);
  const QSignalSpy spy_text_edit(&m_text_edit, &QTextEdit::textChanged);

  mock_listener_t widget(m_text_edit_item);
  EXPECT_CALL(widget, OnPropertyChanged(::testing::_)).Times(1);

  const QString expected_text = {R"RAW(line1
line2
)RAW"};

  m_text_edit.setText(expected_text);
  EXPECT_EQ(spy_text_edit.count(), 1);
  EXPECT_EQ(spy_check_box.count(), 0);
  EXPECT_EQ(m_text_edit_item->GetText(), std::vector<std::string>({"line1", "line2"}));
}

TEST_F(TextEditControllerTest, UpdateItemOnCheckBoxChange)
{
  auto controller = CreateController();
  controller->SetItem(m_text_edit_item);

  const QSignalSpy spy_check_box(&m_check_box, &QCheckBox::stateChanged);
  const QSignalSpy spy_text_edit(&m_text_edit, &QTextEdit::textChanged);

  mock_listener_t widget(m_text_edit_item);
  EXPECT_CALL(widget, OnPropertyChanged(::testing::_)).Times(1);

  m_check_box.setCheckState(Qt::Unchecked);
  EXPECT_EQ(spy_text_edit.count(), 0);
  EXPECT_EQ(spy_check_box.count(), 1);
  EXPECT_EQ(m_text_edit_item->GetText(), std::vector<std::string>({}));
  EXPECT_FALSE(m_text_edit_item->IsEditorEnabled());
  EXPECT_FALSE(m_text_edit.isEnabled());
}

}  // namespace oac_tree_gui::test
