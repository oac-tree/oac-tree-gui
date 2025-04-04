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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/composer/workspace_editor_action_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/procedure_preamble_items.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>
#include <testutils/mock_workspace_editor_context.h>
#include <testutils/test_utils.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

/**
 * @brief Testing WorkspaceEditorActionHandler with focus on undo scenario.
 */
class WorkspaceEditorActionHandlerUndoTest : public ::testing::Test
{
public:
  WorkspaceEditorActionHandlerUndoTest()
  {
    m_procedure_item = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  WorkspaceItem* GetWorkspaceItem() const { return m_procedure_item->GetWorkspace(); }

  /**
   * @brief Creates action handler.
   *
   * @param selection Currently selected variable or one of its leafs.
   */
  std::unique_ptr<WorkspaceEditorActionHandler> CreateActionHandler(mvvm::SessionItem* selection)
  {
    return m_mock_context.CreateActionHandler(GetWorkspaceItem(), {selection});
  }

  /**
   * @brief Returns plugins defined in procedure's preamble.
   */
  std::vector<std::string> GetPluginPaths() const
  {
    return m_procedure_item->GetPreambleItem()->GetPluginPaths();
  }

  ProcedureItem* m_procedure_item{nullptr};
  SequencerModel m_model;
  test::MockWorkspaceEditorContext m_mock_context;
};

TEST_F(WorkspaceEditorActionHandlerUndoTest, InsertLocalVariabledUndoRedo)
{
  m_model.SetUndoEnabled(true);

  auto handler = CreateActionHandler(nullptr);

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));

  handler->AddVariable(LocalVariableItem::GetStaticType());
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  m_model.GetCommandStack()->Undo();
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 0);

  m_model.GetCommandStack()->Redo();
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
}

//! Checking that preamble is correctly updated when EPICS variables is added and removed.
TEST_F(WorkspaceEditorActionHandlerUndoTest, InsertEpicsVariabled)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto handler = CreateActionHandler(nullptr);

  EXPECT_TRUE(GetPluginPaths().empty());

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));

  handler->AddVariable(PvAccessServerVariableItem::GetStaticType());
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  // we get plugin file name in preamble
  EXPECT_EQ(GetPluginPaths(), std::vector<std::string>({oac_tree_gui::GetPluginFileName(
                                  domainconstants::kEpicsPVXSPluginName)}));

  // removing variable
  m_mock_context.m_current_selection = {GetWorkspaceItem()->GetVariables().at(0)};
  handler->RemoveVariable();

  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 0);
  EXPECT_TRUE(GetPluginPaths().empty());
}

//! Checking that preamble is correctly updated when EPICS variables is added and removed.
TEST_F(WorkspaceEditorActionHandlerUndoTest, InsertEpicsVariabledUndoRedo)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  m_model.SetUndoEnabled(true);
  auto handler = CreateActionHandler(nullptr);

  EXPECT_TRUE(GetPluginPaths().empty());

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));

  handler->AddVariable(PvAccessServerVariableItem::GetStaticType());
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  EXPECT_EQ(GetPluginPaths(), std::vector<std::string>({oac_tree_gui::GetPluginFileName(
                                  domainconstants::kEpicsPVXSPluginName)}));

  m_model.GetCommandStack()->Undo();
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 0);
  EXPECT_TRUE(GetPluginPaths().empty());

  m_model.GetCommandStack()->Redo();
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
  EXPECT_EQ(GetPluginPaths(), std::vector<std::string>({oac_tree_gui::GetPluginFileName(
                                  domainconstants::kEpicsPVXSPluginName)}));
}

}  // namespace oac_tree_gui::test
