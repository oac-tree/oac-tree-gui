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

#include "oac_tree_gui/views/composer/procedure_splittable_editor_widget.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/views/composer/procedure_composer_combo_panel.h>

#include <sup/gui/app/null_command_service.h>

#include <mvvm/standarditems/container_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QSplitter>

Q_DECLARE_METATYPE(const oac_tree_gui::ProcedureItem*)

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

  /**
   * @brief Returns QSettings key holding the state of the procedure editor tabs.
   *
   * Defined in procedure_splittable_editor_widget.cpp
   */
  static QString GetProcedureEditorTabStateKey()
  {
    return "ProcedureSplittableEditorWidget/procedure_tabs";
  }

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

  // model is not set, the creation of the panel is possible
  EXPECT_NO_THROW(editor.CreatePanel());
  EXPECT_EQ(splitter->count(), 1);
}

// TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanelBeforeModelSet)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   EXPECT_EQ(splitter->count(), 0);

//   // model is not set, the creation of the panel is possible
//   EXPECT_NO_THROW(editor.CreatePanel());
//   EXPECT_EQ(splitter->count(), 1);
//   EXPECT_EQ(editor.GetFocusWidget()->GetModel(), nullptr);

//   editor.SetModel(&m_model);
//   EXPECT_EQ(editor.GetFocusWidget()->GetModel(), &m_model);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanelAfterModelSet)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);

//   const QSignalSpy signal_spy(
//       &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

//   editor.SetModel(&m_model);

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   EXPECT_EQ(splitter->count(), 0);

//   editor.CreatePanel();
//   EXPECT_EQ(splitter->count(), 1);
//   EXPECT_EQ(editor.GetFocusWidget()->GetModel(), &m_model);

//   ASSERT_NE(splitter->widget(0), nullptr);
//   EXPECT_EQ(splitter->widget(0), editor.GetFocusWidget());

//   EXPECT_EQ(signal_spy.count(), 0);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanelAndClosePanel)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();

//   const QSignalSpy signal_spy(
//       &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

//   editor.CreatePanel();
//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   EXPECT_EQ(splitter->count(), 2);
//   EXPECT_EQ(splitter->widget(1), editor.GetFocusWidget());
//   auto old_focus_widget = editor.GetFocusWidget();
//   EXPECT_FALSE(old_focus_widget->isHidden());

//   editor.ClosePanel(nullptr);
//   ASSERT_EQ(splitter->count(), 2);

//   editor.ClosePanel(editor.GetFocusWidget());

//   EXPECT_EQ(splitter->count(), 2);  // because deletion is deferred
//   EXPECT_TRUE(old_focus_widget->isHidden());
//   EXPECT_EQ(splitter->widget(0), editor.GetFocusWidget());

//   EXPECT_EQ(signal_spy.count(), 0);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanelBetweenTwoPanels)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.CreatePanel();
//   editor.SetModel(&m_model);

//   const QSignalSpy signal_spy(
//       &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

//   // create second panel in addition to initial one
//   editor.CreatePanel();

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   ASSERT_EQ(splitter->count(), 2);
//   auto first_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(0));
//   auto second_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));
//   editor.SetFocusWidget(first_widget);
//   EXPECT_EQ(first_widget, editor.GetFocusWidget());

//   // create third panel
//   editor.CreatePanel(/*after_widget=*/first_widget);

//   EXPECT_EQ(splitter->count(), 3);
//   EXPECT_NE(first_widget, editor.GetFocusWidget());
//   EXPECT_NE(second_widget, editor.GetFocusWidget());
//   EXPECT_EQ(splitter->widget(0), first_widget);
//   EXPECT_EQ(splitter->widget(1), editor.GetFocusWidget());
//   EXPECT_EQ(splitter->widget(2), second_widget);

//   auto prev_focus_widget = editor.GetFocusWidget();

//   // remove first panel
//   editor.ClosePanel(first_widget);
//   EXPECT_EQ(splitter->count(), 3);  // because deletion is deferred

//   EXPECT_TRUE(first_widget->isHidden());
//   EXPECT_EQ(prev_focus_widget, editor.GetFocusWidget());

//   ASSERT_EQ(signal_spy.count(), 0);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanelViaWidgetSignal)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();

//   const QSignalSpy signal_spy(
//       &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   ASSERT_EQ(splitter->count(), 1);

//   auto focus_widget = editor.GetFocusWidget();
//   ASSERT_NE(focus_widget, nullptr);
//   emit focus_widget->splitViewRequest();
//   EXPECT_EQ(splitter->count(), 2);
//   EXPECT_EQ(splitter->widget(1), editor.GetFocusWidget());

//   EXPECT_EQ(signal_spy.count(), 0);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, ClosePanelViaWidgetSignal)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();

//   const QSignalSpy signal_spy(
//       &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

//   editor.CreatePanel();
//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   ASSERT_EQ(splitter->count(), 2);

//   auto focus_widget = editor.GetFocusWidget();
//   emit focus_widget->closeViewRequest();
//   EXPECT_EQ(splitter->count(), 2);  // because deletion is deferred
//   EXPECT_TRUE(focus_widget->isHidden());
//   EXPECT_EQ(splitter->widget(0), editor.GetFocusWidget());

//   EXPECT_EQ(signal_spy.count(), 0);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, FocusRequestViaSignals)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();

//   const QSignalSpy signal_spy(
//       &editor, &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

//   editor.CreatePanel();
//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   ASSERT_EQ(splitter->count(), 2);

//   auto first_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(0));
//   auto second_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));

//   editor.SetFocusWidget(first_widget);
//   EXPECT_EQ(first_widget, editor.GetFocusWidget());

//   emit second_widget->panelFocusRequest();
//   EXPECT_EQ(second_widget, editor.GetFocusWidget());

//   EXPECT_EQ(signal_spy.count(), 0);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, SetProcedureToWidgetInFocus)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();

//   QSignalSpy signal_spy(&editor,
//                         &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

//   auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
//   procedure->SetDisplayName("Test procedure");

//   editor.CreatePanel();
//   editor.CreatePanel();

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   ASSERT_EQ(splitter->count(), 3);
//   auto first_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(0));
//   auto second_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));
//   auto third_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(2));

//   editor.SetFocusWidget(second_widget);

//   EXPECT_EQ(second_widget, editor.GetFocusWidget());
//   editor.SetProcedure(procedure);

//   ASSERT_EQ(signal_spy.count(), 1);
//   EXPECT_EQ(signal_spy.takeFirst().at(0).value<const ProcedureItem*>(), procedure);

//   EXPECT_EQ(second_widget->GetCurrentProcedure(), procedure);
//   editor.SetFocusWidget(third_widget);

//   ASSERT_EQ(signal_spy.count(), 1);
//   EXPECT_EQ(signal_spy.takeFirst().at(0).value<const ProcedureItem*>(), nullptr);

//   EXPECT_EQ(third_widget->GetCurrentProcedure(), nullptr);
//   editor.SetProcedure(procedure);

//   EXPECT_EQ(first_widget->GetCurrentProcedure(), nullptr);
//   EXPECT_EQ(second_widget->GetCurrentProcedure(), procedure);
//   EXPECT_EQ(third_widget->GetCurrentProcedure(), procedure);

//   ASSERT_EQ(signal_spy.count(), 1);
//   EXPECT_EQ(signal_spy.takeFirst().at(0).value<const ProcedureItem*>(), procedure);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, CheckSameProcedureInNewlyCreatedWidget)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();

//   QSignalSpy signal_spy(&editor,
//                         &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

//   auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
//   procedure->SetDisplayName("Test procedure");
//   editor.CreatePanel();
//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   ASSERT_EQ(splitter->count(), 2);
//   auto first_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(0));
//   auto second_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));
//   editor.SetFocusWidget(first_widget);
//   EXPECT_EQ(first_widget, editor.GetFocusWidget());
//   editor.SetProcedure(procedure);
//   EXPECT_EQ(first_widget->GetCurrentProcedure(), procedure);
//   editor.CreatePanel(/*after_widget=*/first_widget);
//   EXPECT_EQ(splitter->count(), 3);
//   auto third_widget = dynamic_cast<ProcedureComposerComboPanel*>(splitter->widget(1));
//   EXPECT_EQ(third_widget->GetCurrentProcedure(), procedure);

//   EXPECT_EQ(signal_spy.count(), 1);
//   EXPECT_EQ(signal_spy.takeFirst().at(0).value<const ProcedureItem*>(), procedure);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, ReadSettingsWhenSplitterNonEmpty)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   ASSERT_EQ(splitter->count(), 1);

//   // Expecting no calls as splitter is non-empty
//   EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);
//   editor.ReadSettings(m_mock_read_func.AsStdFunction());

//   // this should create one panel by default
//   ASSERT_EQ(splitter->count(), 1);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, GetComposerViewInfoForSinglePannelWithNoProcedure)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();

//   const auto view_info = editor.GetComposerViewInfo();
//   EXPECT_TRUE(view_info.splitter_state.empty());
//   ASSERT_EQ(view_info.editor_info_list.size(), 1);
//   EXPECT_TRUE(view_info.editor_info_list[0].procedure_id.empty());
//   EXPECT_EQ(view_info.editor_info_list[0].editor_type, ProcedureEditorType::kInstructionTree);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, GetComposerViewInfoForThreeProcedures)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();
//   editor.CreatePanel();
//   editor.CreatePanel();

//   auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
//   auto procedure1 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
//   auto procedure2 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

//   editor.GetWidgetAt(0)->SetProcedure(procedure0);
//   editor.GetWidgetAt(1)->SetProcedure(procedure1);
//   editor.GetWidgetAt(2)->SetProcedure(procedure2);

//   editor.GetWidgetAt(0)->SetProcedureEditorType(ProcedureEditorType::kInstructionTree);
//   editor.GetWidgetAt(1)->SetProcedureEditorType(ProcedureEditorType::kWorkspace);
//   editor.GetWidgetAt(2)->SetProcedureEditorType(ProcedureEditorType::kNodeEditor);

//   const auto view_info = editor.GetComposerViewInfo();
//   EXPECT_FALSE(view_info.splitter_state.empty());
//   ASSERT_EQ(view_info.editor_info_list.size(), 3);
//   EXPECT_EQ(view_info.editor_info_list[0].procedure_id, procedure0->GetIdentifier());
//   EXPECT_EQ(view_info.editor_info_list[0].editor_type, ProcedureEditorType::kInstructionTree);
//   EXPECT_EQ(view_info.editor_info_list[1].procedure_id, procedure1->GetIdentifier());
//   EXPECT_EQ(view_info.editor_info_list[1].editor_type, ProcedureEditorType::kWorkspace);
//   EXPECT_EQ(view_info.editor_info_list[2].procedure_id, procedure2->GetIdentifier());
//   EXPECT_EQ(view_info.editor_info_list[2].editor_type, ProcedureEditorType::kNodeEditor);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, ReadInvalidSettingsWhenSplitterEmpty)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   ASSERT_EQ(splitter->count(), 0);

//   // Expecting called 3 times (number of panels, tab state, splitter state).
//   // Returning invalid variants.
//   EXPECT_CALL(m_mock_read_func, Call(GetPanelCountKey())).Times(1);
//   EXPECT_CALL(m_mock_read_func, Call(GetSplitterStateKey())).Times(1);
//   EXPECT_CALL(m_mock_read_func, Call(GetProcedureEditorTabStateKey())).Times(1);
//   editor.ReadSettings(m_mock_read_func.AsStdFunction());

//   // this should create one panel by default
//   ASSERT_EQ(splitter->count(), 1);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, ReadSettingsAndCreatePanels)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   EXPECT_EQ(splitter->count(), 0);

//   const std::int32_t expected_panel_count = 3;
//   ON_CALL(m_mock_read_func, Call(GetPanelCountKey()))
//       .WillByDefault(::testing::Return(QVariant::fromValue(expected_panel_count)));

//   QList<std::int32_t> active_tabs;
//   active_tabs.append(static_cast<std::int32_t>(ProcedureEditorType::kInstructionTree));
//   active_tabs.append(static_cast<std::int32_t>(ProcedureEditorType::kWorkspace));
//   active_tabs.append(static_cast<std::int32_t>(ProcedureEditorType::kNodeEditor));
//   ON_CALL(m_mock_read_func, Call(GetProcedureEditorTabStateKey()))
//       .WillByDefault(::testing::Return(QVariant::fromValue(active_tabs)));

//   EXPECT_CALL(m_mock_read_func, Call(GetPanelCountKey())).Times(1);
//   EXPECT_CALL(m_mock_read_func, Call(GetSplitterStateKey())).Times(1);
//   EXPECT_CALL(m_mock_read_func, Call(GetProcedureEditorTabStateKey())).Times(1);

//   editor.ReadSettings(m_mock_read_func.AsStdFunction());

//   EXPECT_EQ(editor.GetWidgetAt(0)->GetProcedureEditorType(), ProcedureEditorType::kInstructionTree);
//   EXPECT_EQ(editor.GetWidgetAt(1)->GetProcedureEditorType(), ProcedureEditorType::kWorkspace);
//   EXPECT_EQ(editor.GetWidgetAt(2)->GetProcedureEditorType(), ProcedureEditorType::kNodeEditor);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, SetComposerViewInfoForTwoPanels)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);

//   auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
//   auto procedure1 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   EXPECT_EQ(splitter->count(), 0);
//   ComposerViewInfo view_info;
//   view_info.splitter_state = "";  // left empty
//   view_info.editor_info_list = {{procedure0->GetIdentifier(), ProcedureEditorType::kNodeEditor},
//                                 {procedure1->GetIdentifier(), ProcedureEditorType::kWorkspace}};

//   editor.SetComposerViewInfo(view_info);
//   EXPECT_EQ(splitter->count(), 2);
//   EXPECT_EQ(editor.GetWidgetAt(0)->GetCurrentProcedure(), procedure0);
//   EXPECT_EQ(editor.GetWidgetAt(0)->GetProcedureEditorType(), ProcedureEditorType::kNodeEditor);
//   EXPECT_EQ(editor.GetWidgetAt(1)->GetCurrentProcedure(), procedure1);
//   EXPECT_EQ(editor.GetWidgetAt(1)->GetProcedureEditorType(), ProcedureEditorType::kWorkspace);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, SetComposerViewInfoWhenProcedureIdAbsent)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);

//   auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

//   auto splitter = editor.findChild<QSplitter*>();
//   ASSERT_NE(splitter, nullptr);
//   EXPECT_EQ(splitter->count(), 0);
//   ComposerViewInfo view_info;
//   view_info.splitter_state = "";  // left empty
//   view_info.editor_info_list = {{"non-existing-id1", ProcedureEditorType::kNodeEditor},
//                                 {"non-existing-id2", ProcedureEditorType::kWorkspace}};

//   // opens our only procedure in both panels
//   editor.SetComposerViewInfo(view_info);
//   EXPECT_EQ(splitter->count(), 2);
//   EXPECT_EQ(editor.GetWidgetAt(0)->GetCurrentProcedure(), procedure0);
//   EXPECT_EQ(editor.GetWidgetAt(0)->GetProcedureEditorType(), ProcedureEditorType::kNodeEditor);
//   EXPECT_EQ(editor.GetWidgetAt(1)->GetCurrentProcedure(), procedure0);
//   EXPECT_EQ(editor.GetWidgetAt(1)->GetProcedureEditorType(), ProcedureEditorType::kWorkspace);
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, WriteSettingsForThreePanels)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();
//   editor.CreatePanel();
//   editor.CreatePanel();

//   editor.GetWidgetAt(0)->SetProcedureEditorType(ProcedureEditorType::kInstructionTree);
//   editor.GetWidgetAt(1)->SetProcedureEditorType(ProcedureEditorType::kWorkspace);
//   editor.GetWidgetAt(2)->SetProcedureEditorType(ProcedureEditorType::kNodeEditor);

//   EXPECT_CALL(m_mock_write_func, Call(GetPanelCountKey(), ::testing::_))
//       .Times(1)
//       .WillOnce(::testing::Invoke(
//           [](const QString& key, const QVariant& value)
//           {
//             EXPECT_EQ(key, GetPanelCountKey());
//             EXPECT_EQ(value.toInt(), 3);
//           }));
//   EXPECT_CALL(m_mock_write_func, Call(GetSplitterStateKey(), ::testing::_)).Times(1);
//   EXPECT_CALL(m_mock_write_func, Call(GetProcedureEditorTabStateKey(), ::testing::_))
//       .Times(1)
//       .WillOnce(::testing::Invoke(
//           [](const QString& key, const QVariant& value)
//           {
//             EXPECT_EQ(key, GetProcedureEditorTabStateKey());
//             const auto tab_indexes = value.value<QList<std::int32_t>>();
//             EXPECT_EQ(tab_indexes.size(), 3);
//             EXPECT_EQ(tab_indexes[0],
//                       static_cast<std::int32_t>(ProcedureEditorType::kInstructionTree));
//             EXPECT_EQ(tab_indexes[1], static_cast<std::int32_t>(ProcedureEditorType::kWorkspace));
//             EXPECT_EQ(tab_indexes[2], static_cast<std::int32_t>(ProcedureEditorType::kNodeEditor));
//           }));
//   editor.WriteSettings(m_mock_write_func.AsStdFunction());
// }

// TEST_F(ProcedureSplittableEditorWidgetTest, InsertInstructionFromToolbox)
// {
//   ProcedureSplittableEditorWidget editor(m_command_service);
//   editor.SetModel(&m_model);
//   editor.CreatePanel();

//   auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
//   editor.SetProcedure(procedure);

//   editor.InsertInstructionFromToolBox(domainconstants::kWaitInstructionType);
//   EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructionCount(), 1);
// }

}  // namespace oac_tree_gui::test
