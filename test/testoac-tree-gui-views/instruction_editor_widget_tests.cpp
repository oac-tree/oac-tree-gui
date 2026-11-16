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

#include "oac_tree_gui/views/composer/instruction_editor_widget.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/views/composer/instruction_editor_tree_view.h>

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/app/null_command_service.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>
#include <testutils/test_settings_manager.h>

#include <QApplication>
#include <QScrollBar>

namespace oac_tree_gui::test
{

//! Tests for InstructionEditorWidget class.
class InstructionEditorWidgetTest : public ::testing::Test
{
public:
  InstructionEditorWidgetTest()
  {
    auto procedure = m_model.InsertItem<ProcedureItem>();
    m_container = procedure->GetInstructionContainer();

    // a long sequence, so the tree does not fit the viewport
    auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType, m_container);
    for (int i = 0; i < kWaitCount; ++i)
    {
      (void)InsertInstruction(domainconstants::kWaitInstructionType, sequence);
    }
    m_nested_sequence = InsertInstruction(domainconstants::kSequenceInstructionType, sequence);
    m_target = InsertInstruction(domainconstants::kWaitInstructionType, m_nested_sequence);

    m_widget.resize(400, 400);
    m_widget.show();
    QApplication::processEvents();
    m_widget.SetInstructionContainer(m_container);
    QApplication::processEvents();

    m_tree_view = m_widget.findChild<InstructionEditorTreeView*>();
  }

  //! Returns the view index of the selected instruction.
  QModelIndex GetSelectedIndex() const
  {
    const auto indexes = m_tree_view->selectionModel()->selectedIndexes();
    return indexes.empty() ? QModelIndex() : indexes.front();
  }

  //! Returns true if the index is inside the tree viewport.
  bool IsInViewport(const QModelIndex& index) const
  {
    const auto rect = m_tree_view->visualRect(index);
    return rect.isValid() && m_tree_view->viewport()->rect().contains(rect);
  }

  static constexpr int kWaitCount = 50;

  mvvm::ApplicationModel m_model;
  sup::gui::NullCommandService m_command_service;
  TestSettingsManager m_settings_manager;
  sup::gui::AppViewContext m_context{m_command_service, m_settings_manager};
  InstructionEditorWidget m_widget{m_context};
  InstructionContainerItem* m_container{nullptr};
  InstructionItem* m_nested_sequence{nullptr};
  InstructionItem* m_target{nullptr};
  InstructionEditorTreeView* m_tree_view{nullptr};
};

//! A selection inside a collapsed parent, outside the viewport, is expanded and scrolled to.
TEST_F(InstructionEditorWidgetTest, SetSelectedInstructionsExpandsAndScrolls)
{
  ASSERT_NE(m_tree_view, nullptr);
  m_tree_view->collapseAll();
  m_tree_view->scrollToTop();

  m_widget.SetSelectedInstructions({m_target});

  const auto index = GetSelectedIndex();
  ASSERT_TRUE(index.isValid());
  EXPECT_EQ(m_widget.GetSelectedInstructions(), std::vector<InstructionItem*>({m_target}));
  EXPECT_TRUE(m_tree_view->isExpanded(index.parent()));
  EXPECT_TRUE(m_tree_view->isExpanded(index.parent().parent()));
  EXPECT_TRUE(IsInViewport(index));
}

//! An empty selection does not scroll.
TEST_F(InstructionEditorWidgetTest, EmptySelectionDoesNotScroll)
{
  ASSERT_NE(m_tree_view, nullptr);
  auto scroll_bar = m_tree_view->verticalScrollBar();
  ASSERT_GT(scroll_bar->maximum(), 0);
  scroll_bar->setValue(scroll_bar->maximum() / 2);
  const auto position = scroll_bar->value();

  m_widget.SetSelectedInstructions({});

  EXPECT_EQ(scroll_bar->value(), position);
}

}  // namespace oac_tree_gui::test
