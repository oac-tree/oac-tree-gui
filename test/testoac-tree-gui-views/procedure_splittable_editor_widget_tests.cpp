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

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/app/null_command_service.h>
#include <sup/gui/settings/in_memory_settings_storage.h>
#include <sup/gui/settings/settings_manager.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>

#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QSplitter>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
Q_DECLARE_METATYPE(mvvm::ItemSelection)
#endif

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
  ProcedureSplittableEditorWidgetTest()
  {
    m_settings_manager.SetStorage(sup::gui::SettingsScope::kApplication, &m_settings_storage);
    m_settings_manager.SetStorage(sup::gui::SettingsScope::kProject, &m_project_storage);
  }

  //! Stores composer view info in application settings.
  void StoreViewInfo(const ComposerViewInfo& view_info)
  {
    m_settings_manager.Set(kComposerPanelsSettingsGroup, view_info,
                           sup::gui::SettingsScope::kApplication);
  }

  //! Returns editor types of all panels.
  static std::vector<ProcedureEditorType> GetEditorTypes(
      const ProcedureSplittableEditorWidget& editor)
  {
    std::vector<ProcedureEditorType> result;
    for (auto panel : editor.GetController()->GetPanels())
    {
      result.push_back(panel->GetMainEditor<ProcedureComposerTabWidget>()->GetEditorType());
    }
    return result;
  }

  SequencerModel m_model;
  sup::gui::NullCommandService m_command_service;
  sup::gui::InMemorySettingsStorage m_settings_storage;
  sup::gui::InMemorySettingsStorage m_project_storage;
  sup::gui::SettingsManager m_settings_manager;
  sup::gui::AppViewContext m_context{m_command_service, m_settings_manager};
};

TEST_F(ProcedureSplittableEditorWidgetTest, InitialState)
{
  ProcedureSplittableEditorWidget editor(m_context);

  ASSERT_NE(editor.GetController(), nullptr);

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 1);

  const std::vector<ProcedureEditorType> expected_types({ProcedureEditorType::kInstructionTree});
  EXPECT_EQ(GetEditorTypes(editor), expected_types);
  EXPECT_EQ(editor.GetController()->GetPanels().at(0)->GetCurrentItem(), nullptr);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CreatePanel)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.show();

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 1);

  // model is not set, the creation of the panel is possible
  EXPECT_NO_THROW(editor.CreatePanel());
  EXPECT_EQ(splitter->count(), 2);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetModelAfterPanelCreation)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.show();

  editor.SetModel(&m_model);
  editor.CreatePanel();

  auto panels = editor.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  EXPECT_EQ(panels.at(0)->GetModel(), &m_model);
  EXPECT_EQ(panels.at(1)->GetModel(), &m_model);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetProcedureToWidgetInFocus)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);

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
  EXPECT_EQ(mvvm::test::TakeValue<mvvm::ItemSelection>(signal_spy), mvvm::ItemSelection(procedure));

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
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);

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

TEST_F(ProcedureSplittableEditorWidgetTest, GetComposerViewInfoForSinglePannelWithNoProcedure)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);

  const auto view_info = editor.GetComposerViewInfo();
  EXPECT_TRUE(view_info.splitter_state.empty());
  ASSERT_EQ(view_info.editor_info_list.size(), 1);
  EXPECT_TRUE(view_info.editor_info_list[0].procedure_id.empty());
  EXPECT_EQ(view_info.editor_info_list[0].editor_type, ProcedureEditorType::kInstructionTree);
}

TEST_F(ProcedureSplittableEditorWidgetTest, GetComposerViewInfoForThreeProcedures)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);
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

TEST_F(ProcedureSplittableEditorWidgetTest, SetComposerViewInfoForTwoPanels)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);

  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  auto procedure1 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 1);
  ComposerViewInfo view_info;
  view_info.splitter_state = "";  // left empty
  view_info.editor_info_list = {{procedure0->GetIdentifier(), ProcedureEditorType::kNodeEditor},
                                {procedure1->GetIdentifier(), ProcedureEditorType::kWorkspace}};

  // default panel is replaced
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
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);

  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  EXPECT_EQ(splitter->count(), 1);
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

TEST_F(ProcedureSplittableEditorWidgetTest, SetComposerViewInfoReplacesPanels)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);
  editor.CreatePanel();
  editor.CreatePanel();

  auto splitter = editor.findChild<QSplitter*>();
  ASSERT_NE(splitter, nullptr);
  ASSERT_EQ(splitter->count(), 3);

  editor.SetComposerViewInfo({"", {{"", ProcedureEditorType::kXmlPanel}}});

  EXPECT_EQ(splitter->count(), 1);
  const std::vector<ProcedureEditorType> expected_types({ProcedureEditorType::kXmlPanel});
  EXPECT_EQ(GetEditorTypes(editor), expected_types);
  EXPECT_EQ(editor.GetController()->GetFocusWidget(), editor.GetController()->GetPanels().at(0));
}

TEST_F(ProcedureSplittableEditorWidgetTest, RestorePanelsFromSettings)
{
  StoreViewInfo({"",
                 {{"", ProcedureEditorType::kWorkspace},
                  {"", ProcedureEditorType::kXmlPanel},
                  {"", ProcedureEditorType::kNodeEditor}}});

  ProcedureSplittableEditorWidget editor(m_context);

  const std::vector<ProcedureEditorType> expected_types({ProcedureEditorType::kWorkspace,
                                                         ProcedureEditorType::kXmlPanel,
                                                         ProcedureEditorType::kNodeEditor});
  EXPECT_EQ(GetEditorTypes(editor), expected_types);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CollectAndRestore)
{
  auto editor = std::make_unique<ProcedureSplittableEditorWidget>(m_context);
  editor->CreatePanel();
  auto panels = editor->GetController()->GetPanels();
  panels.at(0)->GetMainEditor<ProcedureComposerTabWidget>()->SetEditorType(
      ProcedureEditorType::kXmlPanel);
  panels.at(1)->GetMainEditor<ProcedureComposerTabWidget>()->SetEditorType(
      ProcedureEditorType::kWorkspace);

  m_settings_manager.Collect(sup::gui::SettingsScope::kApplication);
  editor.reset();

  ProcedureSplittableEditorWidget new_editor(m_context);

  const std::vector<ProcedureEditorType> expected_types(
      {ProcedureEditorType::kXmlPanel, ProcedureEditorType::kWorkspace});
  EXPECT_EQ(GetEditorTypes(new_editor), expected_types);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetModelAssignsStoredProcedures)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  auto procedure1 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  StoreViewInfo({"",
                 {{procedure1->GetIdentifier(), ProcedureEditorType::kInstructionTree},
                  {procedure0->GetIdentifier(), ProcedureEditorType::kWorkspace}}});

  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);

  auto panels = editor.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  EXPECT_EQ(panels.at(0)->GetCurrentItem(), procedure1);
  EXPECT_EQ(panels.at(1)->GetCurrentItem(), procedure0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, SetModelAssignsFirstProcedureWhenIdAbsent)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  (void)m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  StoreViewInfo({"", {{"non-existing-id", ProcedureEditorType::kInstructionTree}}});

  ProcedureSplittableEditorWidget editor(m_context);
  editor.CreatePanel();  // panel without stored info
  editor.SetModel(&m_model);

  auto panels = editor.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  EXPECT_EQ(panels.at(0)->GetCurrentItem(), procedure0);
  EXPECT_EQ(panels.at(1)->GetCurrentItem(), procedure0);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CollectStoresComposerViewInfo)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);
  editor.CreatePanel();

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  auto panels = editor.GetController()->GetPanels();
  panels.at(1)->SetItem(procedure);
  panels.at(1)->GetMainEditor<ProcedureComposerTabWidget>()->SetEditorType(
      ProcedureEditorType::kXmlPanel);

  m_settings_manager.Collect(sup::gui::SettingsScope::kApplication);

  const auto view_info = m_settings_manager.Get<ComposerViewInfo>(
      kComposerPanelsSettingsGroup, sup::gui::SettingsScope::kApplication);
  EXPECT_EQ(view_info, editor.GetComposerViewInfo());
  ASSERT_EQ(view_info.editor_info_list.size(), 2);
  EXPECT_EQ(view_info.editor_info_list[1].procedure_id, procedure->GetIdentifier());
  EXPECT_EQ(view_info.editor_info_list[1].editor_type, ProcedureEditorType::kXmlPanel);
}

TEST_F(ProcedureSplittableEditorWidgetTest, ProjectNotificationReplacesPanels)
{
  auto procedure0 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  auto procedure1 = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);
  editor.CreatePanel();
  editor.CreatePanel();

  const ComposerViewInfo project_info{
      "",
      {{procedure1->GetIdentifier(), ProcedureEditorType::kWorkspace},
       {procedure0->GetIdentifier(), ProcedureEditorType::kXmlPanel}}};
  sup::gui::WriteToStorage(m_project_storage, kComposerPanelsSettingsGroup, project_info);

  m_settings_manager.NotifyAll(sup::gui::SettingsScope::kProject);

  auto panels = editor.GetController()->GetPanels();
  ASSERT_EQ(panels.size(), 2);
  EXPECT_EQ(panels.at(0)->GetCurrentItem(), procedure1);
  EXPECT_EQ(panels.at(1)->GetCurrentItem(), procedure0);
  const std::vector<ProcedureEditorType> expected_types(
      {ProcedureEditorType::kWorkspace, ProcedureEditorType::kXmlPanel});
  EXPECT_EQ(GetEditorTypes(editor), expected_types);
}

TEST_F(ProcedureSplittableEditorWidgetTest, EmptyProjectNotificationKeepsPanels)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);
  editor.CreatePanel();
  editor.CreatePanel();
  const auto panels = editor.GetController()->GetPanels();

  m_settings_manager.NotifyAll(sup::gui::SettingsScope::kProject);

  EXPECT_EQ(editor.GetController()->GetPanels(), panels);
}

TEST_F(ProcedureSplittableEditorWidgetTest, CollectProjectSettings)
{
  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());

  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);
  editor.CreatePanel();
  auto panels = editor.GetController()->GetPanels();
  panels.at(1)->GetMainEditor<ProcedureComposerTabWidget>()->SetEditorType(
      ProcedureEditorType::kNodeEditor);

  m_settings_manager.Collect(sup::gui::SettingsScope::kProject);

  const auto view_info = m_settings_manager.Get<ComposerViewInfo>(
      kComposerPanelsSettingsGroup, sup::gui::SettingsScope::kProject);
  EXPECT_EQ(view_info, editor.GetComposerViewInfo());
  ASSERT_EQ(view_info.editor_info_list.size(), 2);
  EXPECT_EQ(view_info.editor_info_list[0].procedure_id, procedure->GetIdentifier());
  EXPECT_EQ(view_info.editor_info_list[1].editor_type, ProcedureEditorType::kNodeEditor);

  // application settings are not touched
  EXPECT_TRUE(m_settings_storage.GetKeys().empty());
}

TEST_F(ProcedureSplittableEditorWidgetTest, InsertInstructionFromToolbox)
{
  ProcedureSplittableEditorWidget editor(m_context);
  editor.SetModel(&m_model);

  auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  editor.SetProcedure(procedure);

  editor.InsertInstructionFromToolBox(domainconstants::kWaitInstructionType);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructionCount(), 1);
}

}  // namespace oac_tree_gui::test
