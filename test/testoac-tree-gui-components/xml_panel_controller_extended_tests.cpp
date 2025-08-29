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

#include "oac_tree_gui/components/xml_panel_controller.h"

#include <oac_tree_gui/composer/instruction_editor_action_handler.h>
#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/epics_instruction_items.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/commands/i_command_stack.h>
#include <mvvm/standarditems/container_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_instruction_editor_context.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

/**
 * @brief Testing XmlPanelControllerTest class together with InstructionEditorActionHandler and undo
 * scenario.
 */
class XmlPanelControllerExtendedTest : public ::testing::Test
{
public:
  XmlPanelControllerExtendedTest()
  {
    m_procedure_item = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  std::unique_ptr<XmlPanelController> CreateController()
  {
    return std::make_unique<XmlPanelController>(m_procedure_item, m_mock_send_xml.AsStdFunction(),
                                                m_mock_send_message.AsStdFunction());
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

  InstructionContainerItem* GetInstructionContainer() const
  {
    return m_procedure_item->GetInstructionContainer();
  }

  ProcedureItem* m_procedure_item{nullptr};
  SequencerModel m_model;
  ::testing::MockFunction<XmlPanelController::send_text_func_t> m_mock_send_xml;
  ::testing::MockFunction<XmlPanelController::send_message_func_t> m_mock_send_message;
  test::MockInstructionEditorContext m_mock_context;
};

TEST_F(XmlPanelControllerExtendedTest, XmlGenerationForSequence)
{
  m_model.SetUndoEnabled(true);

  auto handler = CreateActionHandler(nullptr);

  EXPECT_CALL(m_mock_send_xml, Call(::testing::_)).Times(1);
  auto controller = CreateController();

  EXPECT_CALL(m_mock_context, NotifyRequest(testing::_)).Times(1);
  EXPECT_CALL(m_mock_send_xml, Call(::testing::_)).Times(1);
  handler->InsertInstructionAfter(SequenceItem::GetStaticType());
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 1);

  EXPECT_CALL(m_mock_send_xml, Call(::testing::_)).Times(1);
  m_model.GetCommandStack()->Undo();
  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 0);
}

//! Check that update of XML contains correct preamble when EPICS instruction is added to procedure.
TEST_F(XmlPanelControllerExtendedTest, XmlGenerationForEpicsInstruction)
{
  const std::string expected_xml1(R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>
  <Workspace/>
</Procedure>
)RAW");

  const std::string expected_xml2(R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>
  <PvAccessRead timeout="1.0"/>
  <Workspace/>
</Procedure>
)RAW");

  const std::string expected_xml3(R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>
  <Plugin>liboac-tree-pvxs.so</Plugin>
  <PvAccessRead timeout="1.0"/>
  <Workspace/>
</Procedure>
)RAW");

  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  m_model.SetUndoEnabled(true);

  auto handler = CreateActionHandler(nullptr);

  EXPECT_CALL(m_mock_send_xml, Call(expected_xml1)).Times(1);
  auto controller = CreateController();

  EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(0);
  EXPECT_CALL(m_mock_context, NotifyRequest(testing::_)).Times(1);
  EXPECT_CALL(m_mock_send_xml, Call(expected_xml2)).Times(1);
  EXPECT_CALL(m_mock_send_xml, Call(expected_xml3)).Times(1);
  handler->InsertInstructionAfter(mvvm::GetTypeName<PvAccessReadInstructionItem>());

  ASSERT_EQ(GetInstructionContainer()->GetInstructionCount(), 1);

  const std::string expected_xml4(R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>
  <PvAccessRead timeout="1.0"/>
  <Workspace/>
</Procedure>
)RAW");

  const std::string expected_xml5(R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>
  <Workspace/>
</Procedure>
)RAW");

  EXPECT_CALL(m_mock_send_xml, Call(expected_xml4)).Times(1);
  EXPECT_CALL(m_mock_send_xml, Call(expected_xml5)).Times(1);
  m_model.GetCommandStack()->Undo();
}

}  // namespace oac_tree_gui::test
