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
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>
#include <oac_tree_gui/pvmonitor/monitor_model.h>
#include <oac_tree_gui/transform/anyvalue_item_transform_helper.h>
#include <oac_tree_gui/viewmodel/drag_and_drop_helper.h>

#include <sup/gui/components/mime_conversion_helper.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/test/test_helper.h>

#include <sup/dto/anyvalue.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_workspace_editor_context.h>
#include <testutils/test_utils.h>

#include <QMimeData>
#include <QSignalSpy>

Q_DECLARE_METATYPE(mvvm::SessionItem*)

namespace oac_tree_gui::test
{

/**
 * @brief Testing InstructionEditorActionHandler with focus on undo scenario.
 */
class WorkspaceEditorActionHandlerUndoTest : public ::testing::Test
{
public:
  WorkspaceEditorActionHandlerUndoTest()
  {
    m_model.InsertItem<WorkspaceItem>();
    m_model.SetUndoEnabled(true);
  }

  WorkspaceItem* GetWorkspaceItem() const { return m_model.GetWorkspaceItem(); }

  /**
   * @brief Creates action handler.
   *
   * @param selection Currently selected variable or one of its leafs.
   */
  std::unique_ptr<WorkspaceEditorActionHandler> CreateActionHandler(mvvm::SessionItem* selection)
  {
    return m_mock_context.CreateActionHandler(GetWorkspaceItem(), selection);
  }

  MonitorModel m_model;
  test::MockWorkspaceEditorContext m_mock_context;
};

TEST_F(WorkspaceEditorActionHandlerUndoTest, InsertLocalVariabledUndoRedo)
{
  auto handler = CreateActionHandler(nullptr);

  // adding variable
  handler->OnAddVariableRequest(LocalVariableItem::GetStaticType());
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  // undo
  m_model.GetCommandStack()->Undo();
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 0);

  // redo
  m_model.GetCommandStack()->Redo();
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
}

TEST_F(WorkspaceEditorActionHandlerUndoTest, InsertEpicsVariabledUndoRedo)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto handler = CreateActionHandler(nullptr);

  // adding variable
  handler->OnAddVariableRequest(PvAccessServerVariableItem::GetStaticType());
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);

  // undo
  m_model.GetCommandStack()->Undo();
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 0);

  // redo
  m_model.GetCommandStack()->Redo();
  ASSERT_EQ(GetWorkspaceItem()->GetVariableCount(), 1);
}

}  // namespace oac_tree_gui::test
