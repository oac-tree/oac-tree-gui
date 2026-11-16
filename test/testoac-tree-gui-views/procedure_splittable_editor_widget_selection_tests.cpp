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
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/views/composer/instruction_editor_widget.h>
#include <oac_tree_gui/views/composer/procedure_composer_tab_widget.h>
#include <oac_tree_gui/views/nodeeditor/node_editor_widget.h>

#include <sup/gui/app/app_view_context.h>
#include <sup/gui/app/null_command_service.h>
#include <sup/gui/views/dtoeditor/splittable_combo_panel.h>
#include <sup/gui/views/dtoeditor/splittable_editor_controller.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/test_settings_manager.h>

#include <QApplication>

namespace oac_tree_gui::test
{

//! Tests for instruction selection propagation between composer panels.
class ProcedureSplittableEditorWidgetSelectionTest : public ::testing::Test
{
public:
  //! Creates a procedure with a sequence of two waits.
  ProcedureItem* CreateProcedure()
  {
    auto procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
    auto sequence = InsertInstruction(domainconstants::kSequenceInstructionType,
                                      procedure->GetInstructionContainer());
    (void)InsertInstruction(domainconstants::kWaitInstructionType, sequence);
    (void)InsertInstruction(domainconstants::kWaitInstructionType, sequence);
    return procedure;
  }

  //! Returns the waits of a procedure created by CreateProcedure().
  static std::vector<InstructionItem*> GetWaits(ProcedureItem* procedure)
  {
    return procedure->GetInstructionContainer()->GetInstructions().front()->GetInstructions();
  }

  //! Creates panels with the given editor types and procedures, and shows the editor.
  void SetupPanels(const std::vector<std::pair<ProcedureEditorType, ProcedureItem*>>& setup)
  {
    m_editor.SetModel(&m_model);
    for (std::size_t index = 1; index < setup.size(); ++index)
    {
      (void)m_editor.CreatePanel();
    }
    auto panels = m_editor.GetController()->GetPanels();
    for (std::size_t index = 0; index < setup.size(); ++index)
    {
      GetTabWidget(index)->SetEditorType(setup[index].first);
      panels.at(index)->SetItem(setup[index].second);
    }
    m_editor.show();
    QApplication::processEvents();
  }

  //! Returns the main editor of the panel.
  ProcedureComposerTabWidget* GetTabWidget(std::size_t index) const
  {
    return m_editor.GetController()
        ->GetPanels()
        .at(index)
        ->GetMainEditor<ProcedureComposerTabWidget>();
  }

  //! Returns the Instruction Tree of the panel.
  InstructionEditorWidget* GetTree(std::size_t index) const
  {
    return GetTabWidget(index)->findChild<InstructionEditorWidget*>();
  }

  //! Returns the node editor of the panel.
  NodeEditorWidget* GetNodeEditor(std::size_t index) const
  {
    return GetTabWidget(index)->findChild<NodeEditorWidget*>();
  }

  SequencerModel m_model;
  sup::gui::NullCommandService m_command_service;
  TestSettingsManager m_settings_manager;
  sup::gui::AppViewContext m_context{m_command_service, m_settings_manager};
  ProcedureSplittableEditorWidget m_editor{m_context};
};

TEST_F(ProcedureSplittableEditorWidgetSelectionTest, TreeToNodeEditor)
{
  auto procedure = CreateProcedure();
  SetupPanels({{ProcedureEditorType::kInstructionTree, procedure},
               {ProcedureEditorType::kNodeEditor, procedure}});
  const auto waits = GetWaits(procedure);

  GetTree(0)->SetSelectedInstructions({waits.at(0)});

  EXPECT_EQ(GetNodeEditor(1)->GetSelectedInstructions(),
            std::vector<InstructionItem*>({waits.at(0)}));
}

TEST_F(ProcedureSplittableEditorWidgetSelectionTest, NodeEditorToTree)
{
  auto procedure = CreateProcedure();
  SetupPanels({{ProcedureEditorType::kInstructionTree, procedure},
               {ProcedureEditorType::kNodeEditor, procedure}});
  const auto waits = GetWaits(procedure);

  GetNodeEditor(1)->SetSelectedInstructions({waits.at(1)});

  EXPECT_EQ(GetTree(0)->GetSelectedInstructions(), std::vector<InstructionItem*>({waits.at(1)}));
}

TEST_F(ProcedureSplittableEditorWidgetSelectionTest, PanelWithOtherProcedureIsNotTouched)
{
  auto procedure0 = CreateProcedure();
  auto procedure1 = CreateProcedure();
  SetupPanels({{ProcedureEditorType::kInstructionTree, procedure0},
               {ProcedureEditorType::kNodeEditor, procedure0},
               {ProcedureEditorType::kInstructionTree, procedure1}});
  const auto waits0 = GetWaits(procedure0);
  const auto waits1 = GetWaits(procedure1);
  GetTree(2)->SetSelectedInstructions({waits1.at(0)});

  GetTree(0)->SetSelectedInstructions({waits0.at(0)});

  EXPECT_EQ(GetNodeEditor(1)->GetSelectedInstructions(),
            std::vector<InstructionItem*>({waits0.at(0)}));
  EXPECT_EQ(GetTree(2)->GetSelectedInstructions(), std::vector<InstructionItem*>({waits1.at(0)}));
}

TEST_F(ProcedureSplittableEditorWidgetSelectionTest, OneEmissionPerChange)
{
  auto procedure = CreateProcedure();
  SetupPanels({{ProcedureEditorType::kInstructionTree, procedure},
               {ProcedureEditorType::kNodeEditor, procedure}});
  const auto waits = GetWaits(procedure);

  int emission_count{0};
  QObject::connect(GetTabWidget(0), &ProcedureComposerTabWidget::instructionSelectionChanged,
                   [&emission_count](const auto&) { ++emission_count; });

  GetTree(0)->SetSelectedInstructions({waits.at(0)});
  EXPECT_EQ(emission_count, 1);

  GetTree(0)->SetSelectedInstructions({waits.at(0), waits.at(1)});
  EXPECT_EQ(emission_count, 2);
  EXPECT_EQ(GetNodeEditor(1)->GetSelectedInstructions().size(), 2);
}

//! Hiding the node editor destroys its shapes. The other panel keeps its selection.
TEST_F(ProcedureSplittableEditorWidgetSelectionTest, HiddenNodeEditorKeepsOtherSelection)
{
  auto procedure = CreateProcedure();
  SetupPanels({{ProcedureEditorType::kInstructionTree, procedure},
               {ProcedureEditorType::kNodeEditor, procedure}});
  const auto waits = GetWaits(procedure);
  GetTree(0)->SetSelectedInstructions({waits.at(0)});
  ASSERT_EQ(GetNodeEditor(1)->GetSelectedInstructions().size(), 1);

  GetTabWidget(1)->SetEditorType(ProcedureEditorType::kXmlPanel);
  QApplication::processEvents();

  EXPECT_EQ(GetTree(0)->GetSelectedInstructions(), std::vector<InstructionItem*>({waits.at(0)}));
}

}  // namespace oac_tree_gui::test
