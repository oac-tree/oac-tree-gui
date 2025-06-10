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

#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <sup/gui/core/sup_gui_core_exceptions.h>

#include <mvvm/standarditems/container_item.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Testing XmlPanelControllerTest class.
 */
class XmlPanelControllerTest : public ::testing::Test
{
public:
  XmlPanelControllerTest()
  {
    m_procedure_item = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  std::unique_ptr<XmlPanelController> CreateController()
  {
    return std::make_unique<XmlPanelController>(m_procedure_item, m_mock_send_xml.AsStdFunction(),
                                                m_mock_send_message.AsStdFunction());
  }

  ProcedureItem* m_procedure_item{nullptr};
  SequencerModel m_model;
  ::testing::MockFunction<XmlPanelController::send_text_func_t> m_mock_send_xml;
  ::testing::MockFunction<XmlPanelController::send_message_func_t> m_mock_send_message;
};

TEST_F(XmlPanelControllerTest, ConstructorParameter)
{
  EXPECT_THROW(XmlPanelController(nullptr, {}, {}), sup::gui::RuntimeException);
  EXPECT_THROW(XmlPanelController(m_procedure_item, {}, {}), sup::gui::RuntimeException);
  EXPECT_THROW(XmlPanelController(m_procedure_item, [](auto) {}, {}), sup::gui::RuntimeException);
}

TEST_F(XmlPanelControllerTest, CheckInitialUpdate)
{
  const std::string expected_xml(R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>
  <Workspace/>
</Procedure>
)RAW");

  EXPECT_CALL(m_mock_send_xml, Call(expected_xml)).Times(1);
  auto controller = CreateController();
}

TEST_F(XmlPanelControllerTest, NotificationOnProcedureRemoval)
{
  EXPECT_CALL(m_mock_send_xml, Call(::testing::_)).Times(1);
  auto controller = CreateController();

  EXPECT_CALL(m_mock_send_xml, Call(std::string())).Times(1);
  m_model.RemoveItem(m_procedure_item);
}

TEST_F(XmlPanelControllerTest, XmlUpdateOnItemInsert)
{
  const std::string expected_xml(R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>
  <Sequence/>
  <Workspace/>
</Procedure>
)RAW");

  EXPECT_CALL(m_mock_send_xml, Call(::testing::_)).Times(1);
  auto controller = CreateController();

  EXPECT_CALL(m_mock_send_xml, Call(expected_xml)).Times(1);
  m_model.InsertItem<SequenceItem>(m_procedure_item->GetInstructionContainer(),
                                   mvvm::TagIndex::Append());
}

TEST_F(XmlPanelControllerTest, XmlUpdateOnTwoVariableInsert)
{
  const std::string expected_xml1(R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>
  <Workspace>
    <File name="File"/>
  </Workspace>
</Procedure>
)RAW");

  EXPECT_CALL(m_mock_send_xml, Call(::testing::_)).Times(1);
  auto controller = CreateController();

  EXPECT_CALL(m_mock_send_xml, Call(expected_xml1)).Times(1);
  m_model.InsertItem<FileVariableItem>(m_procedure_item->GetWorkspace(), mvvm::TagIndex::Append());

  const std::string expected_xml2(R"RAW(<?xml version="1.0" encoding="UTF-8"?>
<Procedure>
  <Workspace>
    <File name="File0"/>
    <File name="File1"/>
  </Workspace>
</Procedure>
)RAW");

  // adding second variable
  EXPECT_CALL(m_mock_send_xml, Call(expected_xml2)).Times(1);
  auto var1 = m_model.InsertItem<FileVariableItem>(m_procedure_item->GetWorkspace(),
                                                   mvvm::TagIndex::Append());

  // making names of variables the same and expecting error report
  // EXPECT_CALL(m_mock_send_xml, Call(std::string())).Times(1);
  EXPECT_CALL(m_mock_send_message, Call(::testing::_)).Times(1);
  var1->SetDisplayName("File0");
}

}  // namespace oac_tree_gui::test
