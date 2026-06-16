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
#include <oac_tree_gui/views/composer/procedure_composer_tab_widget.h>

#include <sup/gui/app/null_command_service.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

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

  ASSERT_NE(editor.GetController(), nullptr);

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanel)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.show();

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 0);

  // model is not set, the creation of the panel is possible
  EXPECT_NO_THROW(editor.CreatePanel());
  EXPECT_EQ(splitter->count(), 1);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetModelAfterPanelCreation)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.show();

  editor.CreatePanel();
  editor.SetModel(&m_model);
  editor.CreatePanel();

  auto panels = editor.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  EXPECT_EQ(panels.at(0)->GetModel(), &m_model);
  EXPECT_EQ(panels.at(1)->GetModel(), &m_model);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetProcedureToWidgetInFocus)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);
  editor.CreatePanel();

  QSignalSpy signal_spy(&editor,
                        &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  procedure->SetDisplayName("Test procedure");

  editor.CreatePanel();
  editor.CreatePanel();

  auto panels = editor.GetController()->GetPanels();
  auto first_widget = panels.at(0);
  auto second_widget = panels.at(1);
  auto third_widget = panels.at(2);

  editor.GetController()->SetFocusWidget(second_widget);

  editor.SetProcedure(procedure);

  ASSERT_EQ(signal_spy.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<mvvm::ItemSelection>(signal_spy),
            mvvm::ItemSelection(procedure));

  EXPECT_EQ(second_widget->GetCurrentItem(), procedure);

  editor.GetController()->SetFocusWidget(third_widget);

  EXPECT_EQ(third_widget->GetCurrentItem(), nullptr);
  editor.SetProcedure(procedure);

  EXPECT_EQ(first_widget->GetCurrentItem(), nullptr);
  EXPECT_EQ(second_widget->GetCurrentItem(), procedure);
  EXPECT_EQ(third_widget->GetCurrentItem(), procedure);

  // we do not emit signal then same procedure is selected in another focus widget
  ASSERT_EQ(signal_spy.count(), 0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CheckSameProcedureInNewlyCreatedWidget)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);
  editor.CreatePanel();

  QSignalSpy signal_spy(&editor,
                        &ProcedureSplittableEditorWidget::focusWidgetProcedureSelectionChanged);

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  procedure->SetDisplayName("Test procedure");
  editor.CreatePanel();
  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  ASSERT_EQ(splitter->count(), 2);

  auto panels = editor.GetController()->GetPanels();
  auto first_widget = panels.at(0);
  auto second_widget = panels.at(1);

  editor.GetController()->SetFocusWidget(first_widget);

  EXPECT_EQ(first_widget, editor.GetController()->GetFocusWidget());
  editor.SetProcedure(procedure);
  EXPECT_EQ(first_widget->GetCurrentItem(), procedure);

  EXPECT_EQ(signal_spy.count(), 1);

  editor.GetController()->CreatePanel(/*after_widget=*/first_widget);

  EXPECT_EQ(splitter->count(), 3);
  auto third_widget = dynamic_cast<sup::gui::SplittableComboPanel*>(splitter->widget(1));
  EXPECT_EQ(third_widget->GetCurrentItem(), procedure);

  EXPECT_EQ(signal_spy.count(), 1);
}

TEST_F(ProcedureSplittableEditorWidgetTest, ReadSettingsWhenSplitterNonEmpty)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);
  editor.CreatePanel();

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  ASSERT_EQ(splitter->count(), 1);

  // Expecting no calls as splitter is non-empty
  EXPECT_CALL(m_mock_read_func, Call(::testing::_)).Times(0);
  editor.ReadSettings(m_mock_read_func.AsStdFunction());

  // this should create one panel by default
  ASSERT_EQ(splitter->count(), 1);
}

TEST_F(ProcedureSplittableEditorWidgetTest, GetComposerViewInfoForSinglePannelWithNoProcedure)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);
  editor.CreatePanel();

  const auto view_info = editor.GetComposerViewInfo();
  EXPECT_TRUE(view_info.splitter_state.empty());
  ASSERT_EQ(view_info.editor_info_list.size(), 1);
  EXPECT_TRUE(view_info.editor_info_list[0].procedure_id.empty());
  EXPECT_EQ(view_info.editor_info_list[0].editor_type, ProcedureEditorType::kInstructionTree);
}

TEST_F(ProcedureSplittableEditorWidgetTest, GetComposerViewInfoForThreeProcedures)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);
  editor.CreatePanel();
  editor.CreatePanel();
  editor.CreatePanel();

  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  auto procedure1 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  auto procedure2 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  auto panels = editor.GetController()->GetPanels();

  panels.at(0)->SetItem(procedure0);
  panels.at(1)->SetItem(procedure1);
  panels.at(2)->SetItem(procedure2);

  auto editor0 = panels.at(0)->GetMainEditor<ProcedureComposerTabWidget>();
  auto editor1 = panels.at(1)->GetMainEditor<ProcedureComposerTabWidget>();
  auto editor2 = panels.at(2)->GetMainEditor<ProcedureComposerTabWidget>();

  editor0->SetEditorType(ProcedureEditorType::kInstructionTree);
  editor1->SetEditorType(ProcedureEditorType::kWorkspace);
  editor2->SetEditorType(ProcedureEditorType::kNodeEditor);

  const auto view_info = editor.GetComposerViewInfo();
  EXPECT_FALSE(view_info.splitter_state.empty());
  ASSERT_EQ(view_info.editor_info_list.size(), 3);
  EXPECT_EQ(view_info.editor_info_list[0].procedure_id, procedure0->GetIdentifier());
  EXPECT_EQ(view_info.editor_info_list[0].editor_type, ProcedureEditorType::kInstructionTree);
  EXPECT_EQ(view_info.editor_info_list[1].procedure_id, procedure1->GetIdentifier());
  EXPECT_EQ(view_info.editor_info_list[1].editor_type, ProcedureEditorType::kWorkspace);
  EXPECT_EQ(view_info.editor_info_list[2].procedure_id, procedure2->GetIdentifier());
  EXPECT_EQ(view_info.editor_info_list[2].editor_type, ProcedureEditorType::kNodeEditor);
}

TEST_F(ProcedureSplittableEditorWidgetTest, ReadInvalidSettingsWhenSplitterEmpty)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  ASSERT_EQ(splitter->count(), 0);

  // Expecting called 3 times (number of panels, tab state, splitter state).
  // Returning invalid variants.
  EXPECT_CALL(m_mock_read_func, Call(GetPanelCountKey())).Times(1);
  EXPECT_CALL(m_mock_read_func, Call(GetSplitterStateKey())).Times(1);
  EXPECT_CALL(m_mock_read_func, Call(GetProcedureEditorTabStateKey())).Times(1);
  editor.ReadSettings(m_mock_read_func.AsStdFunction());

  // this should create one panel by default
  ASSERT_EQ(splitter->count(), 1);
}

TEST_F(ProcedureSplittableEditorWidgetTest, ReadSettingsAndCreatePanels)
{
  ProcedureSplittableEditorWidget editor(m_command_service);

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 0);

  const std::int32_t expected_panel_count = 3;
  ON_CALL(m_mock_read_func, Call(GetPanelCountKey()))
      .WillByDefault(::testing::Return(QVariant::fromValue(expected_panel_count)));

  QList<std::int32_t> active_tabs;
  active_tabs.append(static_cast<std::int32_t>(ProcedureEditorType::kInstructionTree));
  active_tabs.append(static_cast<std::int32_t>(ProcedureEditorType::kWorkspace));
  active_tabs.append(static_cast<std::int32_t>(ProcedureEditorType::kNodeEditor));
  ON_CALL(m_mock_read_func, Call(GetProcedureEditorTabStateKey()))
      .WillByDefault(::testing::Return(QVariant::fromValue(active_tabs)));

  EXPECT_CALL(m_mock_read_func, Call(GetPanelCountKey())).Times(1);
  EXPECT_CALL(m_mock_read_func, Call(GetSplitterStateKey())).Times(1);
  EXPECT_CALL(m_mock_read_func, Call(GetProcedureEditorTabStateKey())).Times(1);

  editor.ReadSettings(m_mock_read_func.AsStdFunction());

  auto panels = editor.GetController()->GetPanels();
  auto editor0 = panels.at(0)->GetMainEditor<ProcedureComposerTabWidget>();
  auto editor1 = panels.at(1)->GetMainEditor<ProcedureComposerTabWidget>();
  auto editor2 = panels.at(2)->GetMainEditor<ProcedureComposerTabWidget>();

  EXPECT_EQ(editor0->GetEditorType(), ProcedureEditorType::kInstructionTree);
  EXPECT_EQ(editor1->GetEditorType(), ProcedureEditorType::kWorkspace);
  EXPECT_EQ(editor2->GetEditorType(), ProcedureEditorType::kNodeEditor);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetComposerViewInfoForTwoPanels)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  auto procedure1 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 0);
  ComposerViewInfo view_info;
  view_info.splitter_state = "";  // left empty
  view_info.editor_info_list = {{procedure0->GetIdentifier(), ProcedureEditorType::kNodeEditor},
                                {procedure1->GetIdentifier(), ProcedureEditorType::kWorkspace}};

  editor.SetComposerViewInfo(view_info);
  EXPECT_EQ(splitter->count(), 2);

  auto panels = editor.GetController()->GetPanels();
  auto editor0 = panels.at(0)->GetMainEditor<ProcedureComposerTabWidget>();
  auto editor1 = panels.at(1)->GetMainEditor<ProcedureComposerTabWidget>();

  EXPECT_EQ(panels.at(0)->GetCurrentItem(), procedure0);
  EXPECT_EQ(editor0->GetEditorType(), ProcedureEditorType::kNodeEditor);
  EXPECT_EQ(panels.at(1)->GetCurrentItem(), procedure1);
  EXPECT_EQ(editor1->GetEditorType(), ProcedureEditorType::kWorkspace);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetComposerViewInfoWhenProcedureIdAbsent)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);

  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 0);
  ComposerViewInfo view_info;
  view_info.splitter_state = "";  // left empty
  view_info.editor_info_list = {{"non-existing-id1", ProcedureEditorType::kNodeEditor},
                                {"non-existing-id2", ProcedureEditorType::kWorkspace}};

  // opens our only procedure in both panels
  editor.SetComposerViewInfo(view_info);
  EXPECT_EQ(splitter->count(), 2);

  auto panels = editor.GetController()->GetPanels();
  auto editor0 = panels.at(0)->GetMainEditor<ProcedureComposerTabWidget>();
  auto editor1 = panels.at(1)->GetMainEditor<ProcedureComposerTabWidget>();

  EXPECT_EQ(panels.at(0)->GetCurrentItem(), procedure0);
  EXPECT_EQ(editor0->GetEditorType(), ProcedureEditorType::kNodeEditor);
  EXPECT_EQ(panels.at(1)->GetCurrentItem(), procedure0);
  EXPECT_EQ(editor1->GetEditorType(), ProcedureEditorType::kWorkspace);
}

TEST_F(ProcedureSplittableEditorWidgetTest, WriteSettingsForThreePanels)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);
  editor.CreatePanel();
  editor.CreatePanel();
  editor.CreatePanel();

  auto panels = editor.GetController()->GetPanels();
  auto editor0 = panels.at(0)->GetMainEditor<ProcedureComposerTabWidget>();
  auto editor1 = panels.at(1)->GetMainEditor<ProcedureComposerTabWidget>();
  auto editor2 = panels.at(2)->GetMainEditor<ProcedureComposerTabWidget>();

  editor0->SetEditorType(ProcedureEditorType::kInstructionTree);
  editor1->SetEditorType(ProcedureEditorType::kWorkspace);
  editor2->SetEditorType(ProcedureEditorType::kNodeEditor);

  EXPECT_CALL(m_mock_write_func, Call(GetPanelCountKey(), ::testing::_))
      .Times(1)
      .WillOnce(::testing::Invoke(
          [](const QString& key, const QVariant& value)
          {
            EXPECT_EQ(key, GetPanelCountKey());
            EXPECT_EQ(value.toInt(), 3);
          }));
  EXPECT_CALL(m_mock_write_func, Call(GetSplitterStateKey(), ::testing::_)).Times(1);
  EXPECT_CALL(m_mock_write_func, Call(GetProcedureEditorTabStateKey(), ::testing::_))
      .Times(1)
      .WillOnce(::testing::Invoke(
          [](const QString& key, const QVariant& value)
          {
            EXPECT_EQ(key, GetProcedureEditorTabStateKey());
            const auto tab_indexes = value.value<QList<std::int32_t>>();
            EXPECT_EQ(tab_indexes.size(), 3);
            EXPECT_EQ(tab_indexes[0],
                      static_cast<std::int32_t>(ProcedureEditorType::kInstructionTree));
            EXPECT_EQ(tab_indexes[1], static_cast<std::int32_t>(ProcedureEditorType::kWorkspace));
            EXPECT_EQ(tab_indexes[2], static_cast<std::int32_t>(ProcedureEditorType::kNodeEditor));
          }));
  editor.WriteSettings(m_mock_write_func.AsStdFunction());
}

TEST_F(ProcedureSplittableEditorWidgetTest, InsertInstructionFromToolbox)
{
  ProcedureSplittableEditorWidget editor(m_command_service);
  editor.SetModel(&m_model);
  editor.CreatePanel();

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  editor.SetProcedure(procedure);

  editor.InsertInstructionFromToolBox(domainconstants::kWaitInstructionType);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructionCount(), 1);
}

}  // namespace oac_tree_gui::test
