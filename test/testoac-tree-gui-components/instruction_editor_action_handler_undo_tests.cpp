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

#include "oac_tree_gui/composer/instruction_editor_action_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/epics_instruction_items.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/item_constants.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/procedure_preamble_items.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_instruction_editor_context.h>
#include <testutils/test_utils.h>

#include <QMimeData>
#include <QSignalSpy>

Q_DECLARE_METATYPE(mvvm::SessionItem*)

namespace oac_tree_gui::test
{

/**
 * @brief Testing InstructionEditorActionHandler with focus on undo scenario.
 */
class InstructionEditorActionHandlerUndoTest : public ::testing::Test
{
public:
  InstructionEditorActionHandlerUndoTest()
  {
    m_procedure_item = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  InstructionContainerItem* GetInstructionContainer() const
  {
    return m_procedure_item->GetInstructionContainer();
  }

  /**
   * @brief Creates action handler.
   *
   * @param selection Instruction which will be reported as user selection.
   */
  std::unique_ptr<InstructionEditorActionHandler> CreateActionHandler(InstructionItem* selection)
  {
    return m_mock_context.CreateActionHandler(m_procedure_item->GetInstructionContainer(),
                                              {selection});
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
  test::MockInstructionEditorContext m_mock_context;
};

TEST_F(InstructionEditorActionHandlerUndoTest, InsertSequenceUndoRedo)
{
  m_model.SetUndoEnabled(true);

  auto handler = CreateActionHandler(nullptr);

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));
  handler->InsertInstructionAfter(SequenceItem::GetStaticType());
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 1);

  m_model.GetCommandStack()->Undo();
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 0);

  m_model.GetCommandStack()->Redo();
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 1);
}

//! Checking that preamble is correctly updated when EPICS instruction is added and removed.
TEST_F(InstructionEditorActionHandlerUndoTest, InsertEpicsInstructionAfter)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  auto handler = CreateActionHandler(nullptr);

  EXPECT_TRUE(GetPluginPaths().empty());

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));
  handler->InsertInstructionAfter(PvAccessReadInstructionItem::GetStaticType());
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 1);

  // we get plugin name in preamble
  EXPECT_EQ(GetPluginPaths(), std::vector<std::string>({oac_tree_gui::GetPluginFileName(
                                  domainconstants::kEpicsPVXSPluginName)}));

  m_mock_context.m_current_selection = {GetInstructionContainer()->GetInstructions().at(0)};
  handler->RemoveInstruction();

  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 0);
  EXPECT_TRUE(GetPluginPaths().empty());
}

//! Checking that preamble is correctly updated when EPICS variables is added and removed.
//! Adds undo/redo in the scenario.
TEST_F(InstructionEditorActionHandlerUndoTest, InsertEpicsInstructionAfterAndUndo)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  m_model.SetUndoEnabled(true);
  auto handler = CreateActionHandler(nullptr);

  EXPECT_TRUE(GetPluginPaths().empty());

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_));
  handler->InsertInstructionAfter(PvAccessReadInstructionItem::GetStaticType());
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 1);

  EXPECT_EQ(GetPluginPaths(), std::vector<std::string>({oac_tree_gui::GetPluginFileName(
                                  domainconstants::kEpicsPVXSPluginName)}));

  m_model.GetCommandStack()->Undo();
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 0);
  EXPECT_TRUE(GetPluginPaths().empty());

  m_model.GetCommandStack()->Redo();
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 1);
  EXPECT_EQ(GetPluginPaths(), std::vector<std::string>({oac_tree_gui::GetPluginFileName(
                                  domainconstants::kEpicsPVXSPluginName)}));
}

//! Inserts EPICS instruction into sequence and undo.
TEST_F(InstructionEditorActionHandlerUndoTest, InsertEpicsInstructionIntoSequencerAndUndo)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  m_model.SetUndoEnabled(true);
  auto handler = CreateActionHandler(nullptr);

  EXPECT_TRUE(GetPluginPaths().empty());

  EXPECT_CALL(m_mock_context, SelectRequest(testing::_)).Times(2);
  handler->InsertInstructionAfter(SequenceItem::GetStaticType());
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 1);
  EXPECT_TRUE(GetPluginPaths().empty());

  // inserting EPICS instruction into a sequence
  auto inserted_sequence = GetInstructionContainer()->GetInstructions().at(0);
  m_mock_context.m_current_selection = {inserted_sequence};

  handler->InsertInstructionInto(PvAccessReadInstructionItem::GetStaticType());
  ASSERT_EQ(inserted_sequence->GetInstructions().size(), 1);

  EXPECT_EQ(GetPluginPaths(), std::vector<std::string>({oac_tree_gui::GetPluginFileName(
                                  domainconstants::kEpicsPVXSPluginName)}));

  m_model.GetCommandStack()->Undo();
  ASSERT_EQ(inserted_sequence->GetInstructions().size(), 0);

  EXPECT_TRUE(GetPluginPaths().empty());

  m_model.GetCommandStack()->Redo();
  ASSERT_EQ(inserted_sequence->GetInstructions().size(), 1);
  EXPECT_EQ(GetPluginPaths(), std::vector<std::string>({oac_tree_gui::GetPluginFileName(
                                  domainconstants::kEpicsPVXSPluginName)}));
}

}  // namespace oac_tree_gui::test
