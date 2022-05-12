/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/monitor/jobmanager.h"

#include "sequencergui/model/procedureitems.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/model/standardvariableitems.h"
#include "sequencergui/monitor/jobcontext.h"
#include "sequencergui/monitor/messagepanel.h"

#include "mvvm/model/modelutils.h"
#include "mvvm/standarditems/containeritem.h"

#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QTest>

using namespace sequencergui;

//! Tests for JobManager.

class JobManagerTest : public ::testing::Test
{
public:
  JobManagerTest() : m_model(std::make_unique<SequencerModel>())
  {
    qRegisterMetaType<InstructionItem*>("InstructionItem*");
    SetupCopyProcedure();
    SetupIncludeProcedure();
  }

  void SetupCopyProcedure()
  {
    m_copy_procedure = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
    auto copy = m_model->InsertItem<CopyItem>(m_copy_procedure->GetInstructionContainer());
    copy->SetInput("var0");
    copy->SetOutput("var1");

    auto var0 = m_model->InsertItem<LocalVariableItem>(m_copy_procedure->GetWorkspace());
    var0->SetName("var0");
    var0->SetJsonType(R"({"type":"uint32"})");
    var0->SetJsonValue("42");

    auto var1 = m_model->InsertItem<LocalVariableItem>(m_copy_procedure->GetWorkspace());
    var1->SetName("var1");
    var1->SetJsonType(R"({"type":"uint32"})");
    var1->SetJsonValue("43");
  }

  void SetupIncludeProcedure()
  {
    m_include_procedure = m_model->InsertItem<ProcedureItem>(m_model->GetProcedureContainer());
    auto sequence =
        m_model->InsertItem<SequenceItem>(m_include_procedure->GetInstructionContainer());
    sequence->SetName("MySequence");
    m_model->InsertItem<WaitItem>(sequence);
    m_model->InsertItem<WaitItem>(sequence);
    m_model->InsertItem<WaitItem>(sequence);

    auto repeat = m_model->InsertItem<RepeatItem>(m_include_procedure->GetInstructionContainer());
    repeat->SetRepeatCount(-1);
    repeat->SetIsRootFlag(true);
    auto include = m_model->InsertItem<IncludeItem>(repeat);
    include->SetPath("MySequence");
  }

  std::unique_ptr<SequencerModel> m_model;
  ProcedureItem* m_include_procedure{nullptr};
  ProcedureItem* m_copy_procedure{nullptr};
};

TEST_F(JobManagerTest, InitialState)
{
  JobManager manager;
  manager.SetModel(m_model.get());
  EXPECT_FALSE(manager.GetCurrentContext());
}

//! Set first procedure to the JobManager and execute it.

TEST_F(JobManagerTest, SetCurrentProcedure)
{
  MessagePanel panel;

  JobManager manager;
  manager.SetMessagePanel(&panel);
  manager.SetModel(m_model.get());
  manager.SetCurrentProcedure(m_copy_procedure);
  manager.onChangeDelayRequest(10);

  QSignalSpy spy_instruction_status(&manager, &JobManager::InstructionStatusChanged);

  auto context = manager.GetCurrentContext();
  ASSERT_TRUE(context != nullptr);

  EXPECT_FALSE(context->IsRunning());
  EXPECT_TRUE(context->GetExpandedModel());

  auto procedure = context->GetExpandedProcedure();
  ASSERT_TRUE(procedure != nullptr);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructions().size(), 1);
  EXPECT_EQ(procedure->GetWorkspace()->GetVariables().size(), 2);

  // starting procedure
  manager.onStartProcedureRequest(m_copy_procedure);
  EXPECT_TRUE(context->IsRunning());

  // We are testing here queued signals, need special waiting to let procedure complete
  QTest::qWait(100);
  EXPECT_FALSE(context->IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  // variables inside are changed
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(context->GetExpandedModel());
  EXPECT_EQ(vars_inside.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars_inside.at(1)->GetJsonValue(), std::string("42"));

  // variables at original model remained unchanged
  auto inside = mvvm::utils::FindItems<LocalVariableItem>(m_model.get());
  EXPECT_EQ(inside.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(inside.at(1)->GetJsonValue(), std::string("43"));
}
