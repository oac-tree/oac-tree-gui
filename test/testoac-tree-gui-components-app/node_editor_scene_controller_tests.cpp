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

#include "oac_tree_gui/nodeeditor/objects/node_editor_scene_controller.h"

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/instruction_item.h>
#include <oac_tree_gui/model/instruction_item_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/nodeeditor/objects/node_graphics_scene.h>
#include <oac_tree_gui/nodeeditor/sequencer_align_utils.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/model/application_model.h>
#include <mvvm/utils/container_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/test_node_editor_helper.h>

#include <QCoreApplication>
#include <QSignalSpy>

namespace oac_tree_gui::test
{

//! Tests for NodeEditorSceneController class.
class NodeEditorSceneControllerTest : public ::testing::Test
{
public:
  //! Creates an active controller.
  std::unique_ptr<NodeEditorSceneController> CreateController(NodeEditorMode editor_mode)
  {
    auto result = std::make_unique<NodeEditorSceneController>(editor_mode, &m_scene,
                                                              m_mock_message.AsStdFunction());
    result->SetActive(true);
    return result;
  }

  //! Returns instructions with a shape on the scene.
  std::vector<InstructionItem*> GetSceneInstructions() const
  {
    return FindSceneInstructions(m_scene);
  }

  //! Inserts an instruction away from the origin, so it needs no alignment.
  static InstructionItem* InsertAt(const std::string& domain_type, mvvm::SessionItem* parent,
                                   double x, double y)
  {
    auto result = InsertInstruction(domain_type, parent);
    result->SetX(x);
    result->SetY(y);
    return result;
  }

  //! A procedure with a root "Main" holding Includes, and a SubTree "Sub" with a child "Inner".
  struct SubTreeProcedure
  {
    ProcedureItem* procedure{nullptr};
    InstructionItem* main{nullptr};
    InstructionItem* sub{nullptr};
    InstructionItem* inner{nullptr};
  };

  //! Creates SubTreeProcedure.
  SubTreeProcedure CreateSubTreeProcedure()
  {
    SubTreeProcedure result;
    result.procedure = m_model.InsertItem<ProcedureItem>();
    auto container = result.procedure->GetInstructionContainer();
    result.main = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
    result.main->SetName("Main");
    result.main->SetIsRootFlag(true);
    result.sub = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);
    result.sub->SetName("Sub");
    result.inner = InsertAt(domainconstants::kWaitInstructionType, result.sub, 20.0, 30.0);
    result.inner->SetName("Inner");
    return result;
  }

  //! Inserts an Include with the given path into the parent.
  static InstructionItem* InsertInclude(const std::string& path, mvvm::SessionItem* parent)
  {
    auto result = InsertAt(domainconstants::kIncludeInstructionType, parent, 10.0, 20.0);
    result->SetAttribute(domainconstants::kPathAttribute, path);
    return result;
  }

  NodeGraphicsScene m_scene;
  mvvm::ApplicationModel m_model;
  testing::MockFunction<void(const sup::gui::MessageEvent&)> m_mock_message;
};

TEST_F(NodeEditorSceneControllerTest, InitialState)
{
  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  EXPECT_EQ(controller->GetInstructionContainer(), nullptr);
  EXPECT_EQ(controller->GetScope(), nullptr);
  EXPECT_TRUE(controller->GetVisibleTopLevelInstructions().empty());
  EXPECT_TRUE(controller->GetSelectedInstructions().empty());
  EXPECT_TRUE(m_scene.items().empty());
}

//! The read-only viewer shows all top-level instructions and never emits scopeShown.
TEST_F(NodeEditorSceneControllerTest, ViewerShowsAllTopLevelInstructions)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);

  auto controller = CreateController(NodeEditorMode::kNodeViewer);
  const QSignalSpy spy_shown(controller.get(), &NodeEditorSceneController::scopeShown);
  controller->SetProcedure(procedure);
  QCoreApplication::processEvents();

  EXPECT_EQ(controller->GetScope(), nullptr);
  EXPECT_EQ(controller->GetVisibleTopLevelInstructions(),
            std::vector<InstructionItem*>({sequence0, sequence1}));
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence0, sequence1}));
  EXPECT_EQ(spy_shown.count(), 0);

  controller->SetScope(sequence0);
  EXPECT_EQ(controller->GetScope(), nullptr);
}

//! The default scope is the root. Switching the scope shows another SubTree.
TEST_F(NodeEditorSceneControllerTest, DefaultScopeAndSwitch)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto wait0 = InsertAt(domainconstants::kWaitInstructionType, sequence0, 10.0, 20.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);
  sequence1->SetIsRootFlag(true);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  const QSignalSpy spy_changed(controller.get(), &NodeEditorSceneController::scopeChanged);
  const QSignalSpy spy_shown(controller.get(), &NodeEditorSceneController::scopeShown);

  controller->SetProcedure(procedure);
  EXPECT_EQ(controller->GetInstructionContainer(), container);
  EXPECT_EQ(controller->GetScope(), sequence1);
  EXPECT_EQ(controller->GetVisibleTopLevelInstructions(),
            std::vector<InstructionItem*>({sequence1}));
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence1}));
  EXPECT_EQ(spy_changed.count(), 1);

  // scopeShown is deferred
  EXPECT_EQ(spy_shown.count(), 0);
  QCoreApplication::processEvents();
  EXPECT_EQ(spy_shown.count(), 1);

  controller->SetScope(sequence0);
  EXPECT_EQ(controller->GetScope(), sequence0);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence0, wait0}));
  EXPECT_EQ(spy_changed.count(), 2);
  QCoreApplication::processEvents();
  EXPECT_EQ(spy_shown.count(), 2);

  // same scope again: nothing happens
  controller->SetScope(sequence0);
  QCoreApplication::processEvents();
  EXPECT_EQ(spy_changed.count(), 2);
  EXPECT_EQ(spy_shown.count(), 2);
}

//! An inactive controller does not populate the scene. The scope is shown on activation.
TEST_F(NodeEditorSceneControllerTest, Activation)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  (void)InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);

  NodeEditorSceneController controller(NodeEditorMode::kNodeEditor, &m_scene,
                                       m_mock_message.AsStdFunction());
  const QSignalSpy spy_shown(&controller, &NodeEditorSceneController::scopeShown);

  controller.SetProcedure(procedure);
  controller.SetScope(sequence1);
  QCoreApplication::processEvents();
  EXPECT_EQ(controller.GetScope(), sequence1);
  EXPECT_TRUE(m_scene.items().empty());
  EXPECT_EQ(spy_shown.count(), 0);

  controller.SetActive(true);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence1}));
  QCoreApplication::processEvents();
  EXPECT_EQ(spy_shown.count(), 1);

  controller.SetActive(false);
  EXPECT_TRUE(m_scene.items().empty());
  EXPECT_TRUE(controller.GetSelectedInstructions().empty());
}

//! Removing the current SubTree leaves no scope. Undo restores it.
TEST_F(NodeEditorSceneControllerTest, RemoveScope)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);
  const auto identifier = sequence1->GetIdentifier();
  m_model.SetUndoEnabled(true);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  controller->SetScope(sequence1);
  const QSignalSpy spy_changed(controller.get(), &NodeEditorSceneController::scopeChanged);

  m_model.RemoveItem(sequence1);
  EXPECT_EQ(controller->GetScope(), nullptr);
  EXPECT_TRUE(GetSceneInstructions().empty());
  EXPECT_EQ(spy_changed.count(), 1);

  m_model.GetCommandStack()->Undo();
  ASSERT_NE(controller->GetScope(), nullptr);
  EXPECT_EQ(controller->GetScope()->GetIdentifier(), identifier);
  EXPECT_EQ(GetSceneInstructions(),
            std::vector<InstructionItem*>({container->GetInstructions().at(1)}));
  EXPECT_EQ(spy_changed.count(), 2);

  // removing a hidden instruction changes nothing
  m_model.RemoveItem(sequence0);
  EXPECT_EQ(controller->GetScope()->GetIdentifier(), identifier);
  EXPECT_EQ(spy_changed.count(), 2);
}

//! When the current SubTree is connected under a loose instruction, the scope follows it. Loose
//! instructions are recorded for the new top-level.
TEST_F(NodeEditorSceneControllerTest, MoveScopeUnderLooseInstruction)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  ASSERT_EQ(controller->GetScope(), sequence0);
  const QSignalSpy spy_changed(controller.get(), &NodeEditorSceneController::scopeChanged);

  auto loose_sequence = InsertAt(domainconstants::kSequenceInstructionType, container, 30.0, 30.0);
  m_model.MoveItem(sequence0, loose_sequence,
                   mvvm::TagIndex::Append(itemconstants::kChildInstructions));

  EXPECT_EQ(controller->GetScope(), loose_sequence);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({loose_sequence, sequence0}));
  EXPECT_EQ(spy_changed.count(), 2);  // no scope after the take, then the new top-level

  // a loose instruction dropped now belongs to the new top-level
  auto wait = InsertAt(domainconstants::kWaitInstructionType, container, 40.0, 40.0);

  controller->SetScope(sequence1);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence1}));

  controller->SetScope(loose_sequence);
  EXPECT_EQ(GetSceneInstructions(),
            std::vector<InstructionItem*>({loose_sequence, sequence0, wait}));
}

//! When the current SubTree is moved into a hidden tree, there is no scope.
TEST_F(NodeEditorSceneControllerTest, MoveScopeIntoHiddenTree)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  (void)InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);
  auto sequence2 = InsertAt(domainconstants::kSequenceInstructionType, container, 30.0, 30.0);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  controller->SetScope(sequence1);

  m_model.MoveItem(sequence1, sequence2, mvvm::TagIndex::Append(itemconstants::kChildInstructions));

  EXPECT_EQ(controller->GetScope(), nullptr);
  EXPECT_TRUE(GetSceneInstructions().empty());

  controller->SetScope(sequence2);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence2, sequence1}));
}

//! ShowInstruction switches to the SubTree of an instruction only if this SubTree is hidden.
TEST_F(NodeEditorSceneControllerTest, ShowInstruction)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);
  auto wait1 = InsertAt(domainconstants::kWaitInstructionType, sequence1, 20.0, 30.0);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  ASSERT_EQ(controller->GetScope(), sequence0);
  QCoreApplication::processEvents();
  const QSignalSpy spy_shown(controller.get(), &NodeEditorSceneController::scopeShown);

  // a loose instruction is visible: no switch
  auto loose = InsertAt(domainconstants::kWaitInstructionType, container, 30.0, 30.0);
  controller->ShowInstruction(*loose);
  EXPECT_EQ(controller->GetScope(), sequence0);

  // a child in a hidden SubTree: switch to its top-level
  controller->ShowInstruction(*wait1);
  EXPECT_EQ(controller->GetScope(), sequence1);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence1, wait1}));
  QCoreApplication::processEvents();
  EXPECT_EQ(spy_shown.count(), 1);

  // already shown: no switch
  controller->ShowInstruction(*wait1);
  QCoreApplication::processEvents();
  EXPECT_EQ(spy_shown.count(), 1);

  // instruction of another procedure: ignored
  auto other_procedure = m_model.InsertItem<ProcedureItem>();
  auto other = InsertAt(domainconstants::kSequenceInstructionType,
                        other_procedure->GetInstructionContainer(), 10.0, 10.0);
  controller->ShowInstruction(*other);
  EXPECT_EQ(controller->GetScope(), sequence1);
}

//! In an empty procedure, an inserted instruction is shown.
TEST_F(NodeEditorSceneControllerTest, EmptyProcedure)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  EXPECT_EQ(controller->GetScope(), nullptr);

  auto wait = InsertAt(domainconstants::kWaitInstructionType, container, 10.0, 10.0);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({wait}));
}

//! Initial alignment runs per SubTree, on its first show. An aligned SubTree is not realigned.
TEST_F(NodeEditorSceneControllerTest, InitialAlignmentPerSubTree)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertInstruction(domainconstants::kSequenceInstructionType, container);
  (void)InsertInstruction(domainconstants::kWaitInstructionType, sequence0);
  auto sequence1 = InsertInstruction(domainconstants::kSequenceInstructionType, container);
  (void)InsertInstruction(domainconstants::kWaitInstructionType, sequence1);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  EXPECT_TRUE(algorithm::RequiresInitialAlignment(*sequence0));
  QCoreApplication::processEvents();

  EXPECT_FALSE(algorithm::RequiresInitialAlignment(*sequence0));
  EXPECT_TRUE(algorithm::RequiresInitialAlignment(*sequence1));

  sequence0->SetX(42.0);

  controller->SetScope(sequence1);
  QCoreApplication::processEvents();
  EXPECT_FALSE(algorithm::RequiresInitialAlignment(*sequence1));

  controller->SetScope(sequence0);
  QCoreApplication::processEvents();
  EXPECT_DOUBLE_EQ(sequence0->GetX(), 42.0);
}

//! A loose instruction is hidden when leaving its SubTree, and shown again on return. It survives
//! deactivation.
TEST_F(NodeEditorSceneControllerTest, LooseInstruction)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  ASSERT_EQ(controller->GetScope(), sequence0);

  auto wait = InsertAt(domainconstants::kWaitInstructionType, container, 30.0, 30.0);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence0, wait}));

  controller->SetActive(false);
  controller->SetActive(true);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence0, wait}));

  controller->SetScope(sequence1);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence1}));

  controller->SetScope(sequence0);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence0, wait}));

  // after SetProcedure, the loose instruction is shown only as its own SubTree
  auto other_procedure = m_model.InsertItem<ProcedureItem>();
  controller->SetProcedure(other_procedure);
  controller->SetProcedure(procedure);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence0}));

  controller->SetScope(wait);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({wait}));
}

//! A drop on the scene inserts an instruction into the current SubTree.
TEST_F(NodeEditorSceneControllerTest, DropInstruction)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);

  emit m_scene.dropInstructionRequested(
      QString::fromStdString(domainconstants::kWaitInstructionType), {42.0, 43.0});

  ASSERT_EQ(container->GetInstructionCount(), 2);
  auto dropped = container->GetInstructions().at(1);
  EXPECT_EQ(GetSceneInstructions(), std::vector<InstructionItem*>({sequence0, dropped}));
}

//! Opening an Include switches to the SubTree of its target. A nested target is selected.
TEST_F(NodeEditorSceneControllerTest, OpenSubTree)
{
  auto [procedure, main, sub, inner] = CreateSubTreeProcedure();
  auto include_sub = InsertInclude("Sub", main);
  auto include_inner = InsertInclude("Sub.Inner", main);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  ASSERT_EQ(controller->GetScope(), main);
  const QSignalSpy spy_center(controller.get(),
                              &NodeEditorSceneController::centerOnSelectionRequested);

  EXPECT_TRUE(controller->GetOpenSubTreeError(*include_sub).empty());
  controller->OpenSubTree(*include_sub);
  EXPECT_EQ(controller->GetScope(), sub);
  EXPECT_TRUE(controller->GetSelectedInstructions().empty());
  EXPECT_EQ(spy_center.count(), 0);

  controller->SetScope(main);
  controller->OpenSubTree(*include_inner);
  EXPECT_EQ(controller->GetScope(), sub);
  EXPECT_EQ(controller->GetSelectedInstructions(), std::vector<InstructionItem*>({inner}));
  EXPECT_EQ(spy_center.count(), 1);

  // the target is in the current SubTree: no switch, selected again
  controller->SetSelectedInstructions({});
  controller->OpenSubTree(*include_inner);
  EXPECT_EQ(controller->GetScope(), sub);
  EXPECT_EQ(controller->GetSelectedInstructions(), std::vector<InstructionItem*>({inner}));
  EXPECT_EQ(spy_center.count(), 2);
}

//! External Include, IncludeProcedure and a path that doesn't resolve can't be opened.
TEST_F(NodeEditorSceneControllerTest, OpenSubTreeErrors)
{
  auto [procedure, main, sub, inner] = CreateSubTreeProcedure();
  auto include_external = InsertInclude("Sub", main);
  include_external->SetAttribute(domainconstants::kFileNameAttribute, std::string("other.xml"));
  auto include_procedure =
      InsertAt(domainconstants::kIncludeProcedureInstructionType, main, 10.0, 20.0);
  include_procedure->SetAttribute(domainconstants::kFileNameAttribute, std::string("other.xml"));
  auto include_missing = InsertInclude("Sub.Missing", main);
  auto include_empty = InsertInclude("", main);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);

  EXPECT_EQ(controller->GetOpenSubTreeError(*main), std::string("Not an Include instruction."));
  EXPECT_EQ(controller->GetOpenSubTreeError(*include_external),
            std::string("Include refers to another file."));
  EXPECT_EQ(controller->GetOpenSubTreeError(*include_procedure),
            std::string("IncludeProcedure refers to another procedure file."));
  EXPECT_EQ(controller->GetOpenSubTreeError(*include_missing),
            std::string("Include path 'Sub.Missing' is not found."));
  EXPECT_EQ(controller->GetOpenSubTreeError(*include_empty), std::string("Include path is empty."));

  EXPECT_CALL(m_mock_message, Call(::testing::_)).Times(4);
  for (const auto* include : {include_external, include_procedure, include_missing, include_empty})
  {
    controller->OpenSubTree(*include);
  }
  EXPECT_EQ(controller->GetScope(), main);
}

//! A double-click on an Include opens its SubTree. On other instructions it selects the branch.
TEST_F(NodeEditorSceneControllerTest, DoubleClick)
{
  auto [procedure, main, sub, inner] = CreateSubTreeProcedure();
  auto include_sub = InsertInclude("Sub", main);
  auto include_missing = InsertInclude("Missing", main);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);

  EXPECT_CALL(m_mock_message, Call(::testing::_)).Times(1);
  emit m_scene.instructionDoubleClick(include_missing);
  EXPECT_EQ(controller->GetScope(), main);

  emit m_scene.instructionDoubleClick(include_sub);
  EXPECT_EQ(controller->GetScope(), sub);

  emit m_scene.instructionDoubleClick(sub);
  EXPECT_TRUE(mvvm::utils::HaveSameElements(controller->GetSelectedInstructions(),
                                            std::vector<InstructionItem*>({sub, inner})));
}

//! The viewer doesn't open SubTrees.
TEST_F(NodeEditorSceneControllerTest, ViewerDoesNotOpenSubTree)
{
  auto [procedure, main, sub, inner] = CreateSubTreeProcedure();
  auto include_sub = InsertInclude("Sub", main);

  auto controller = CreateController(NodeEditorMode::kNodeViewer);
  controller->SetProcedure(procedure);
  controller->OpenSubTree(*include_sub);
  EXPECT_EQ(controller->GetScope(), nullptr);
  EXPECT_EQ(GetSceneInstructions().size(), 4);
}

//! Scope switches are recorded. A switch by SetScope starts a trail, an Include extends it.
//! SetProcedure resets the history to the root.
TEST_F(NodeEditorSceneControllerTest, History)
{
  auto [procedure, main, sub, inner] = CreateSubTreeProcedure();
  auto include_sub = InsertInclude("Sub", main);
  auto other = InsertAt(domainconstants::kSequenceInstructionType,
                        procedure->GetInstructionContainer(), 30.0, 30.0);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  const QSignalSpy spy_history(controller.get(), &NodeEditorSceneController::historyChanged);
  controller->SetProcedure(procedure);
  EXPECT_EQ(spy_history.count(), 1);

  const auto& history = controller->GetHistory();
  ASSERT_EQ(history.GetEntries().size(), 1);
  EXPECT_EQ(history.GetEntries().at(0).scope_identifier, main->GetIdentifier());
  EXPECT_FALSE(controller->CanGoBack());
  EXPECT_FALSE(controller->CanGoForward());

  controller->OpenSubTree(*include_sub);
  ASSERT_EQ(history.GetEntries().size(), 2);
  EXPECT_EQ(history.GetEntries().at(1).scope_identifier, sub->GetIdentifier());
  EXPECT_EQ(history.GetEntries().at(1).include_identifier, include_sub->GetIdentifier());
  EXPECT_EQ(history.GetTrailStart(), 0);
  EXPECT_EQ(spy_history.count(), 2);

  // the current SubTree again: not recorded
  controller->SetScope(sub);
  EXPECT_EQ(history.GetEntries().size(), 2);
  EXPECT_EQ(spy_history.count(), 2);

  controller->SetScope(other);
  ASSERT_EQ(history.GetEntries().size(), 3);
  EXPECT_TRUE(history.GetEntries().at(2).include_identifier.empty());
  EXPECT_EQ(history.GetTrailStart(), 2);
  EXPECT_TRUE(controller->CanGoBack());

  controller->SetProcedure(nullptr);
  EXPECT_TRUE(history.GetEntries().empty());
  controller->SetProcedure(procedure);
  EXPECT_EQ(history.GetEntries().size(), 1);
}

//! Back and Forward walk the history without changing it.
TEST_F(NodeEditorSceneControllerTest, BackAndForward)
{
  auto [procedure, main, sub, inner] = CreateSubTreeProcedure();
  auto include_sub = InsertInclude("Sub", main);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  controller->OpenSubTree(*include_sub);
  const QSignalSpy spy_history(controller.get(), &NodeEditorSceneController::historyChanged);

  controller->GoBack();
  EXPECT_EQ(controller->GetScope(), main);
  EXPECT_EQ(controller->GetHistory().GetCurrentIndex(), 0);
  EXPECT_EQ(controller->GetHistory().GetEntries().size(), 2);
  EXPECT_FALSE(controller->CanGoBack());
  EXPECT_TRUE(controller->CanGoForward());
  EXPECT_EQ(spy_history.count(), 1);

  controller->GoBack();
  EXPECT_EQ(controller->GetScope(), main);
  EXPECT_EQ(spy_history.count(), 1);

  controller->GoForward();
  EXPECT_EQ(controller->GetScope(), sub);
  EXPECT_FALSE(controller->CanGoForward());

  controller->GoToHistoryEntry(0);
  EXPECT_EQ(controller->GetScope(), main);
  EXPECT_EQ(controller->GetHistory().GetCurrentIndex(), 0);

  // a new switch drops forward entries
  controller->SetScope(sub);
  EXPECT_EQ(controller->GetHistory().GetEntries().size(), 2);
  EXPECT_TRUE(controller->GetHistory().GetEntries().at(1).include_identifier.empty());
}

//! Back and Forward skip deleted SubTrees and entries of the current SubTree.
TEST_F(NodeEditorSceneControllerTest, BackSkipsDeletedAndCurrent)
{
  auto procedure = m_model.InsertItem<ProcedureItem>();
  auto container = procedure->GetInstructionContainer();
  auto sequence0 = InsertAt(domainconstants::kSequenceInstructionType, container, 10.0, 10.0);
  auto sequence1 = InsertAt(domainconstants::kSequenceInstructionType, container, 20.0, 20.0);
  auto sequence2 = InsertAt(domainconstants::kSequenceInstructionType, container, 30.0, 30.0);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  controller->SetScope(sequence1);
  controller->SetScope(sequence2);
  controller->SetScope(sequence1);

  // history: sequence0, sequence1, sequence2 (deleted), sequence1 (current)
  m_model.RemoveItem(sequence2);
  EXPECT_EQ(controller->GetScope(), sequence1);
  controller->GoBack();
  EXPECT_EQ(controller->GetScope(), sequence0);
  EXPECT_EQ(controller->GetHistory().GetCurrentIndex(), 0);
  EXPECT_TRUE(controller->CanGoForward());

  // the deleted entry is not shown
  controller->GoToHistoryEntry(2);
  EXPECT_EQ(controller->GetScope(), sequence0);
  EXPECT_EQ(controller->GetHistory().GetCurrentIndex(), 0);

  controller->GoForward();
  EXPECT_EQ(controller->GetScope(), sequence1);
  EXPECT_EQ(controller->GetHistory().GetCurrentIndex(), 1);
}

//! The viewer has no history.
TEST_F(NodeEditorSceneControllerTest, ViewerHasNoHistory)
{
  auto [procedure, main, sub, inner] = CreateSubTreeProcedure();

  auto controller = CreateController(NodeEditorMode::kNodeViewer);
  controller->SetProcedure(procedure);
  EXPECT_TRUE(controller->GetHistory().GetEntries().empty());
  EXPECT_FALSE(controller->CanGoBack());
  EXPECT_FALSE(controller->CanGoForward());
}

//! GoToInstruction shows the SubTree of the instruction, selects it and requests centering. A
//! switch starts a new trail.
TEST_F(NodeEditorSceneControllerTest, GoToInstruction)
{
  auto [procedure, main, sub, inner] = CreateSubTreeProcedure();
  auto include_sub = InsertInclude("Sub", main);

  auto controller = CreateController(NodeEditorMode::kNodeEditor);
  controller->SetProcedure(procedure);
  controller->OpenSubTree(*include_sub);
  ASSERT_EQ(controller->GetScope(), sub);
  const QSignalSpy spy_center(controller.get(),
                              &NodeEditorSceneController::centerOnSelectionRequested);

  controller->GoToInstruction(*include_sub);
  EXPECT_EQ(controller->GetScope(), main);
  EXPECT_EQ(controller->GetSelectedInstructions(), std::vector<InstructionItem*>({include_sub}));
  EXPECT_EQ(spy_center.count(), 1);
  EXPECT_EQ(controller->GetHistory().GetEntries().size(), 3);
  EXPECT_EQ(controller->GetHistory().GetTrailStart(), 2);

  // in the current SubTree: no switch
  controller->GoToInstruction(*main);
  EXPECT_EQ(controller->GetScope(), main);
  EXPECT_EQ(controller->GetSelectedInstructions(), std::vector<InstructionItem*>({main}));
  EXPECT_EQ(controller->GetHistory().GetEntries().size(), 3);
  EXPECT_EQ(spy_center.count(), 2);

  auto viewer = CreateController(NodeEditorMode::kNodeViewer);
  viewer->SetProcedure(procedure);
  viewer->GoToInstruction(*inner);
  EXPECT_TRUE(viewer->GetSelectedInstructions().empty());
}

}  // namespace oac_tree_gui::test
