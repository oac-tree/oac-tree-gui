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

#include "sequencergui/model/applicationmodels.h"
#include "sequencergui/model/instructioncontaineritem.h"
#include "sequencergui/model/jobmodel.h"
#include "sequencergui/model/procedureitem.h"
#include "sequencergui/model/sequencermodel.h"
#include "sequencergui/model/standardinstructionitems.h"
#include "sequencergui/model/standardvariableitems.h"
#include "sequencergui/model/workspaceitem.h"
#include "sequencergui/model/jobitem.h"
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
  JobManagerTest()
  {
    qRegisterMetaType<InstructionItem*>("InstructionItem*");
    SetupCopyProcedure();
    SetupIncludeProcedure();
    m_job_item = m_models.GetJobModel()->InsertItem<JobItem>();
  }

  void SetupCopyProcedure()
  {
    auto model = GetSequencerModel();

    m_copy_procedure = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
    auto copy = model->InsertItem<CopyItem>(m_copy_procedure->GetInstructionContainer());
    copy->SetInput("var0");
    copy->SetOutput("var1");

    auto var0 = model->InsertItem<LocalVariableItem>(m_copy_procedure->GetWorkspace());
    var0->SetName("var0");
    var0->SetJsonType(R"({"type":"uint32"})");
    var0->SetJsonValue("42");

    auto var1 = model->InsertItem<LocalVariableItem>(m_copy_procedure->GetWorkspace());
    var1->SetName("var1");
    var1->SetJsonType(R"({"type":"uint32"})");
    var1->SetJsonValue("43");
  }

  void SetupIncludeProcedure()
  {
    auto model = GetSequencerModel();
    m_include_procedure = model->InsertItem<ProcedureItem>(model->GetProcedureContainer());
    auto sequence = model->InsertItem<SequenceItem>(m_include_procedure->GetInstructionContainer());
    sequence->SetName("MySequence");
    model->InsertItem<WaitItem>(sequence);
    model->InsertItem<WaitItem>(sequence);
    model->InsertItem<WaitItem>(sequence);

    auto repeat = model->InsertItem<RepeatItem>(m_include_procedure->GetInstructionContainer());
    repeat->SetRepeatCount(-1);
    repeat->SetIsRootFlag(true);
    auto include = model->InsertItem<IncludeItem>(repeat);
    include->SetPath("MySequence");
  }

  SequencerModel* GetSequencerModel() { return m_models.GetSequencerModel(); }
  JobModel* GetJobModel() { return m_models.GetJobModel(); }

  ApplicationModels m_models;
  ProcedureItem* m_include_procedure{nullptr};
  ProcedureItem* m_copy_procedure{nullptr};
  JobItem* m_job_item{nullptr};
};

TEST_F(JobManagerTest, InitialState)
{
  JobManager manager;
  EXPECT_FALSE(manager.GetCurrentContext());
}

//! Set first procedure to the JobManager and execute it.

TEST_F(JobManagerTest, SetCurrentProcedure)
{
  MessagePanel panel;
  m_job_item->SetProcedure(m_copy_procedure);

  JobManager manager;
  manager.SetMessagePanel(&panel);
  manager.SetCurrentJob(m_job_item);
  manager.onChangeDelayRequest(10);

  QSignalSpy spy_instruction_status(&manager, &JobManager::InstructionStatusChanged);

  auto context = manager.GetCurrentContext();
  ASSERT_TRUE(context != nullptr);

  EXPECT_FALSE(context->IsRunning());

  auto procedure = context->GetExpandedProcedure();
  ASSERT_TRUE(procedure != nullptr);
  EXPECT_EQ(procedure->GetInstructionContainer()->GetInstructions().size(), 1);
  EXPECT_EQ(procedure->GetWorkspace()->GetVariables().size(), 2);

  // starting procedure
  manager.onStartProcedureRequest(m_job_item);
  EXPECT_TRUE(context->IsRunning());

  // We are testing here queued signals, need special waiting to let procedure complete
  QTest::qWait(100);
  EXPECT_FALSE(context->IsRunning());
  EXPECT_EQ(spy_instruction_status.count(), 2);

  // variables inside are changed
  auto vars_inside = mvvm::utils::FindItems<LocalVariableItem>(GetJobModel());
  EXPECT_EQ(vars_inside.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(vars_inside.at(1)->GetJsonValue(), std::string("42"));

  // variables at original model remained unchanged
  auto inside = mvvm::utils::FindItems<LocalVariableItem>(GetSequencerModel());
  EXPECT_EQ(inside.at(0)->GetJsonValue(), std::string("42"));
  EXPECT_EQ(inside.at(1)->GetJsonValue(), std::string("43"));
}
