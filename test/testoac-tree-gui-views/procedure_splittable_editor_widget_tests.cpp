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

#include "oac_tree_gui/views/composer/procedure_splittable_editor_widget.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/views/composer/procedure_composer_combo_panel.h>
#include <oac_tree_gui/views/composer/procedure_composer_combo_toolbar.h>

#include <sup/gui/app/null_command_service.h>

#include <mvvm/standarditems/container_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>
#include <QSignalSpy>
#include <QSplitter>
#include <QStackedWidget>

Q_DECLARE_METATYPE(oac_tree_gui::ProcedureItem*)

namespace oac_tree_gui::test
{

namespace
{
constexpr auto kNoProcedureSelected = " < no procedure >";
}

/**
 * @brief Tests for ProcedureSplittableEditorWidget class.
 */
class ProcedureSplittableEditorWidgetTest : public ::testing::Test
{
public:
  /**
   * @brief Returns QSettings key holding the number of created panels.
   *
   * Defined in procedure_splittable_editor_widget.cpp
   */
  static QString GetPanelCountKey() { return "ProcedureSplittableEditorWidget/panel_count"; }

  /**
   * @brief Returns QSettings key holding the state of the splitter.
   *
   * Defined in procedure_splittable_editor_widget.cpp
   */
  static QString GetSplitterStateKey() { return "ProcedureSplittableEditorWidget/splitter"; }

  SequencerModel m_model;
  sup::gui::NullCommandService m_command_service;
  ::testing::MockFunction<sup::gui::read_variant_func_t> m_mock_read_func;
  ::testing::MockFunction<sup::gui::write_variant_func_t> m_mock_write_func;
};

TEST_F(ProcedureSplittableEditorWidgetTest, InitialState)
{
  ProcedureSplittableEditorWidget editor(m_command_service);

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 0);

  // model is not set, so creating panel should throw
  EXPECT_THROW(editor.CreatePanel(), RuntimeException);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetModel)
{
  ProcedureSplittableEditorWidget editor(m_command_service);

  const QSignalSpy signal_spy(
      &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  editor.SetModel(&m_model);

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 1);

  ASSERT_NE(splitter->widget(0), nullptr);
  EXPECT_EQ(splitter->widget(0), editor.GetFocusWidget());

  EXPECT_EQ(signal_spy.count(), 0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanelAndClosePanel)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  const QSignalSpy signal_spy(
      &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  editor.CreatePanel();
  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 2);
  EXPECT_EQ(splitter->widget(1), editor.GetFocusWidget());
  auto old_focus_widget = editor.GetFocusWidget();
  EXPECT_FALSE(old_focus_widget->isHidden());

  editor.ClosePanel(nullptr);
  ASSERT_EQ(splitter->count(), 2);

  editor.ClosePanel(editor.GetFocusWidget());

  EXPECT_EQ(splitter->count(), 2);  // because deletion is deferred
  EXPECT_TRUE(old_focus_widget->isHidden());
  EXPECT_EQ(splitter->widget(0), editor.GetFocusWidget());

  EXPECT_EQ(signal_spy.count(), 0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanelBetweenTwoPanels)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  const QSignalSpy signal_spy(
      &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  // create second panel in addition to initial one
  editor.CreatePanel();

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  ASSERT_EQ(splitter->count(), 2);
  auto first_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(0));
  auto second_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));
  editor.SetFocusWidget(first_widget);
  EXPECT_EQ(first_widget, editor.GetFocusWidget());

  // create third panel
  editor.CreatePanel(/*after_widget=*/first_widget);

  EXPECT_EQ(splitter->count(), 3);
  EXPECT_NE(first_widget, editor.GetFocusWidget());
  EXPECT_NE(second_widget, editor.GetFocusWidget());
  EXPECT_EQ(splitter->widget(0), first_widget);
  EXPECT_EQ(splitter->widget(1), editor.GetFocusWidget());
  EXPECT_EQ(splitter->widget(2), second_widget);

  auto prev_focus_widget = editor.GetFocusWidget();

  // remove first panel
  editor.ClosePanel(first_widget);
  EXPECT_EQ(splitter->count(), 3);  // because deletion is deferred

  EXPECT_TRUE(first_widget->isHidden());
  EXPECT_EQ(prev_focus_widget, editor.GetFocusWidget());

  ASSERT_EQ(signal_spy.count(), 0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanelViaWidgetSignal)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  const QSignalSpy signal_spy(
      &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 1);

  auto focus_widget = editor.GetFocusWidget();
  emit focus_widget->splitViewRequest();
  EXPECT_EQ(splitter->count(), 2);
  EXPECT_EQ(splitter->widget(1), editor.GetFocusWidget());

  EXPECT_EQ(signal_spy.count(), 0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, ClosePanelViaWidgetSignal)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  const QSignalSpy signal_spy(
      &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  editor.CreatePanel();
  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 2);

  auto focus_widget = editor.GetFocusWidget();
  emit focus_widget->closeViewRequest();
  EXPECT_EQ(splitter->count(), 2);  // because deletion is deferred
  EXPECT_TRUE(focus_widget->isHidden());
  EXPECT_EQ(splitter->widget(0), editor.GetFocusWidget());

  EXPECT_EQ(signal_spy.count(), 0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, FocusRequestViaSignals)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  const QSignalSpy signal_spy(
      &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  editor.CreatePanel();
  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 2);

  auto first_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(0));
  auto second_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));

  editor.SetFocusWidget(first_widget);
  EXPECT_EQ(first_widget, editor.GetFocusWidget());

  emit second_widget->panelFocusRequest();
  EXPECT_EQ(second_widget, editor.GetFocusWidget());

  EXPECT_EQ(signal_spy.count(), 0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetProcedureToWidgetInFocus)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  QSignalSpy signal_spy(&editor,
                        &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  procedure->SetDisplayName("Test procedure");

  editor.CreatePanel();
  editor.CreatePanel();

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 3);
  auto first_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(0));
  auto second_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));
  auto third_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(2));

  editor.SetFocusWidget(second_widget);

  EXPECT_EQ(second_widget, editor.GetFocusWidget());
  editor.SetProcedure(procedure);

  ASSERT_EQ(signal_spy.count(), 1);
  EXPECT_EQ(signal_spy.takeFirst().at(0).value<ProcedureItem*>(), procedure);

  EXPECT_EQ(second_widget->GetCurrentProcedure(), procedure);
  editor.SetFocusWidget(third_widget);

  ASSERT_EQ(signal_spy.count(), 1);
  EXPECT_EQ(signal_spy.takeFirst().at(0).value<ProcedureItem*>(), nullptr);

  EXPECT_EQ(third_widget->GetCurrentProcedure(), nullptr);
  editor.SetProcedure(procedure);

  EXPECT_EQ(first_widget->GetCurrentProcedure(), nullptr);
  EXPECT_EQ(second_widget->GetCurrentProcedure(), procedure);
  EXPECT_EQ(third_widget->GetCurrentProcedure(), procedure);

  ASSERT_EQ(signal_spy.count(), 1);
  EXPECT_EQ(signal_spy.takeFirst().at(0).value<ProcedureItem*>(), procedure);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CheckSameProcedureInNewlyCreatedWidget)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  QSignalSpy signal_spy(&editor,
                        &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  procedure->SetDisplayName("Test procedure");
  editor.CreatePanel();
  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 2);
  auto first_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(0));
  auto second_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));
  editor.SetFocusWidget(first_widget);
  EXPECT_EQ(first_widget, editor.GetFocusWidget());
  editor.SetProcedure(procedure);
  EXPECT_EQ(first_widget->GetCurrentProcedure(), procedure);
  editor.CreatePanel(/*after_widget=*/first_widget);
  EXPECT_EQ(splitter->count(), 3);
  auto third_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));
  EXPECT_EQ(third_widget->GetCurrentProcedure(), procedure);

  EXPECT_EQ(signal_spy.count(), 1);
  EXPECT_EQ(signal_spy.takeFirst().at(0).value<ProcedureItem*>(), procedure);
}

TEST_F(ProcedureSplittableEditorWidgetTest, ReadSettingsWhenSplitterIsNonEmpty)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 1);

  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);
  editor.ReadSettings(m_mock_read_func.AsStdFunction());
}

TEST_F(ProcedureSplittableEditorWidgetTest, ReadSettingsAndCreatePanels)
{
  // ProcedureSplittableEditorWidget editor(m_command_service);

  // auto splitter = editor.findChild<QSplitter*>();
  // ASSERT_NE(splitter, nullptr);
  // EXPECT_EQ(splitter->count(), 0);

  // const std::int32_t expected_panel_count = 3;
  // ON_CALL(m_mock_read_func, Call(GetPanelCountKey()))
  //     .WillByDefault(::testing::Return(QVariant::fromValue(expected_panel_count)));

  // EXPECT_CALL(m_mock_read_func, Call(GetPanelCountKey())).Times(1);
  // // EXPECT_CALL(m_mock_read_func, Call(GetSplitterStateKey())).Times(1);

  // editor.ReadSettings(m_mock_read_func.AsStdFunction());
}

}  // namespace oac_tree_gui::test
