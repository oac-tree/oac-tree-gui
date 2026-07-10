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

#include "oac_tree_gui/components/job_item_controller.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

class JobItemControllerTest : public ::testing::Test
{
public:
  std::unique_ptr<JobItemController> CreateController()
  {
    return std::make_unique<JobItemController>(m_mock_procedure_call.AsStdFunction());
  }

  ::testing::MockFunction<void(ProcedureItem*)> m_mock_procedure_call;
  mvvm::ApplicationModel m_model;
};

TEST_F(JobItemControllerTest, ConstructFailure)
{
  EXPECT_THROW(JobItemController({}), RuntimeException);
}

//! Controller receives JobItem without expanded procedure, no callbacks are expected.
TEST_F(JobItemControllerTest, JobWithoutProcedure)
{
  auto job = m_model.InsertItem<LocalJobItem>();
  EXPECT_EQ(job->GetExpandedProcedure(), nullptr);

  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);

  auto controller = CreateController();
  EXPECT_EQ(controller->GetItem(), nullptr);

  controller->SetItem(job);
  EXPECT_EQ(controller->GetItem(), job);
}

//! Controller receives JobItem with expanded procedure, callback is expected.
TEST_F(JobItemControllerTest, JobWithExpandedProcedure)
{
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);

  auto job = m_model.InsertItem<LocalJobItem>();
  EXPECT_EQ(job->GetExpandedProcedure(), nullptr);
  auto procedure = std::make_unique<ProcedureItem>();
  auto procedure_ptr = procedure.get();
  (void)m_model.InsertItem(std::move(procedure), job, mvvm::TagIndex::Append());
  EXPECT_EQ(job->GetExpandedProcedure(), procedure_ptr);

  auto controller = CreateController();
  EXPECT_EQ(controller->GetItem(), nullptr);

  // charging controller, notification expected
  EXPECT_CALL(m_mock_procedure_call, Call(procedure_ptr)).Times(1);
  controller->SetItem(job);
}

//! Controller receives JobItem with expanded procedure, callback is expected. Then job is removed,
//! callback is expected.
TEST_F(JobItemControllerTest, JobWithExpandedProcedureWhenNoJob)
{
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);

  auto job = m_model.InsertItem<LocalJobItem>();
  EXPECT_EQ(job->GetExpandedProcedure(), nullptr);
  auto procedure = std::make_unique<ProcedureItem>();
  auto procedure_ptr = procedure.get();
  (void)m_model.InsertItem(std::move(procedure), job, mvvm::TagIndex::Append());
  EXPECT_EQ(job->GetExpandedProcedure(), procedure_ptr);

  auto controller = CreateController();
  EXPECT_EQ(controller->GetItem(), nullptr);

  // charging controller, notification expected
  EXPECT_CALL(m_mock_procedure_call, Call(procedure_ptr)).Times(1);
  controller->SetItem(job);

  // expecting "no procedure" callback after job reset
  EXPECT_CALL(m_mock_procedure_call, Call(nullptr)).Times(1);

  controller->SetItem(nullptr);
  EXPECT_EQ(controller->GetItem(), nullptr);
}

//! Controller receives JobItem without expanded procedure, then procedure is inserted, callback is
//! expected.
TEST_F(JobItemControllerTest, EmptyJobThenInsertExpandedProcedure)
{
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);

  auto job = m_model.InsertItem<LocalJobItem>();
  EXPECT_EQ(job->GetExpandedProcedure(), nullptr);

  auto controller = CreateController();
  EXPECT_EQ(controller->GetItem(), nullptr);

  // do not expect calls since job is empty
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);
  controller->SetItem(job);

  // going to inser procedure, expecting notification
  auto procedure = std::make_unique<ProcedureItem>();
  auto procedure_ptr = procedure.get();
  EXPECT_CALL(m_mock_procedure_call, Call(procedure_ptr)).Times(1);

  (void)m_model.InsertItem(std::move(procedure), job, mvvm::TagIndex::Append());
  EXPECT_EQ(job->GetExpandedProcedure(), procedure_ptr);
}

//! Controller receives JobItem with expanded procedure, callback is expected. Then procedure is
//! removed, callback is expected.
TEST_F(JobItemControllerTest, JobWithExpandedProcedureWhenProcedureRemoved)
{
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);

  auto job = m_model.InsertItem<LocalJobItem>();
  EXPECT_EQ(job->GetExpandedProcedure(), nullptr);
  auto procedure = std::make_unique<ProcedureItem>();
  auto procedure_ptr = procedure.get();
  (void)m_model.InsertItem(std::move(procedure), job, mvvm::TagIndex::Append());
  EXPECT_EQ(job->GetExpandedProcedure(), procedure_ptr);

  auto controller = CreateController();
  EXPECT_EQ(controller->GetItem(), nullptr);

  // charging controller, notification expected
  EXPECT_CALL(m_mock_procedure_call, Call(procedure_ptr)).Times(1);
  controller->SetItem(job);

  // expecting "no procedure" callback after procedure removal
  EXPECT_CALL(m_mock_procedure_call, Call(nullptr)).Times(1);

  mvvm::utils::RemoveItem(*procedure_ptr);
  EXPECT_EQ(controller->GetItem(), job);
  EXPECT_EQ(job->GetExpandedProcedure(), nullptr);
}

TEST_F(JobItemControllerTest, JobWithExpandedProcedureNoCallbacksOnProcedureModification)
{
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);

  auto job = m_model.InsertItem<LocalJobItem>();
  EXPECT_EQ(job->GetExpandedProcedure(), nullptr);
  auto procedure = std::make_unique<ProcedureItem>();
  auto procedure_ptr = procedure.get();
  (void)m_model.InsertItem(std::move(procedure), job, mvvm::TagIndex::Append());
  EXPECT_EQ(job->GetExpandedProcedure(), procedure_ptr);

  auto controller = CreateController();
  EXPECT_EQ(controller->GetItem(), nullptr);

  // charging controller, notification expected
  EXPECT_CALL(m_mock_procedure_call, Call(procedure_ptr)).Times(1);
  controller->SetItem(job);

  // modifying procedure shouldn't cause any notifications
  EXPECT_CALL(m_mock_procedure_call, Call(procedure_ptr)).Times(0);
  auto sequence = m_model.InsertItem<SequenceItem>(procedure_ptr->GetInstructionContainer());
  mvvm::utils::RemoveItem(*sequence);

  // adding new job shouldn't cause any notifications
  EXPECT_CALL(m_mock_procedure_call, Call(procedure_ptr)).Times(0);
  auto job2 = m_model.InsertItem<LocalJobItem>();

  // switching to a new job2 with no procedure, should cause notification
  EXPECT_CALL(m_mock_procedure_call, Call(nullptr)).Times(1);
  controller->SetItem(job2);
}

//! Controller is attached to a job with expanded procedure. Then the procedure is replaced in place
//! (old one removed, new one inserted), as happens on job re-run. Two callbacks are expected:
//! nullptr (on removal) followed by the new procedure (on insertion).
TEST_F(JobItemControllerTest, JobWithExpandedProcedureWhenProcedureReplaced)
{
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);

  auto job = m_model.InsertItem<LocalJobItem>();
  auto procedure = std::make_unique<ProcedureItem>();
  auto procedure_ptr = procedure.get();
  (void)m_model.InsertItem(std::move(procedure), job, mvvm::TagIndex::Append());
  EXPECT_EQ(job->GetExpandedProcedure(), procedure_ptr);

  auto controller = CreateController();

  // charging controller, notification expected
  EXPECT_CALL(m_mock_procedure_call, Call(procedure_ptr)).Times(1);
  controller->SetItem(job);

  // replacing procedure: expect "no procedure" on removal, then the new procedure on insertion
  auto new_procedure = std::make_unique<ProcedureItem>();
  auto new_procedure_ptr = new_procedure.get();
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_mock_procedure_call, Call(nullptr)).Times(1);
    EXPECT_CALL(m_mock_procedure_call, Call(new_procedure_ptr)).Times(1);

    mvvm::utils::RemoveItem(*procedure_ptr);
    (void)m_model.InsertItem(std::move(new_procedure), job, mvvm::TagIndex::Append());
  }
  EXPECT_EQ(job->GetExpandedProcedure(), new_procedure_ptr);
}

//! Controller is switched from one job with an expanded procedure to another job which also has an
//! expanded procedure. Two callbacks are expected: nullptr (on detach from the first job) followed
//! by the second job's procedure (on attach).
TEST_F(JobItemControllerTest, SwitchBetweenTwoJobsWithProcedures)
{
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);

  auto job1 = m_model.InsertItem<LocalJobItem>();
  auto procedure1 = std::make_unique<ProcedureItem>();
  auto procedure1_ptr = procedure1.get();
  (void)m_model.InsertItem(std::move(procedure1), job1, mvvm::TagIndex::Append());

  auto job2 = m_model.InsertItem<LocalJobItem>();
  auto procedure2 = std::make_unique<ProcedureItem>();
  auto procedure2_ptr = procedure2.get();
  (void)m_model.InsertItem(std::move(procedure2), job2, mvvm::TagIndex::Append());

  auto controller = CreateController();

  // charging controller with the first job, notification expected
  EXPECT_CALL(m_mock_procedure_call, Call(procedure1_ptr)).Times(1);
  controller->SetItem(job1);

  // switching to the second job: expect "no procedure" on detach, then job2's procedure on attach
  {
    ::testing::InSequence seq;
    EXPECT_CALL(m_mock_procedure_call, Call(nullptr)).Times(1);
    EXPECT_CALL(m_mock_procedure_call, Call(procedure2_ptr)).Times(1);
    controller->SetItem(job2);
  }
}

//! Controller is detached (via SetItem(nullptr)) from a job which has no expanded procedure.
//! No callbacks are expected, since tracking procedure didn't change.
TEST_F(JobItemControllerTest, DetachFromJobWithoutProcedure)
{
  auto job = m_model.InsertItem<LocalJobItem>();
  EXPECT_EQ(job->GetExpandedProcedure(), nullptr);

  auto controller = CreateController();

  // attaching to an empty job, no callbacks expected
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);
  controller->SetItem(job);

  // detaching: no calls are expected
  EXPECT_CALL(m_mock_procedure_call, Call(nullptr)).Times(0);
  controller->SetItem(nullptr);
}

//! Controller is attached to a job with expanded procedure, then the job itself is removed from the
//! model.
//!
//! Currently no callback is fired and controller->GetItem() keeps a dangling pointer: the listener
//! is attached to the job and only observes events within the job's own subtree, so the job's own
//! removal (an event on the job's parent) never reaches it. This is a known limitation of the
//! mvvm::ItemController base class and will be fixed during its refactoring. Until then the client
//! is expected to detach the controller (SetItem(nullptr)) before the job is removed. This test
//! pins the current behavior: removing the job does not crash and produces no callback.
TEST_F(JobItemControllerTest, JobRemovedFromModel)
{
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);

  auto job = m_model.InsertItem<LocalJobItem>();
  auto procedure = std::make_unique<ProcedureItem>();
  auto procedure_ptr = procedure.get();
  (void)m_model.InsertItem(std::move(procedure), job, mvvm::TagIndex::Append());

  auto controller = CreateController();

  // charging controller, notification expected
  EXPECT_CALL(m_mock_procedure_call, Call(procedure_ptr)).Times(1);
  controller->SetItem(job);

  // removing the job itself: no callback is fired (see comment above)
  EXPECT_CALL(m_mock_procedure_call, Call(::testing::_)).Times(0);
  mvvm::utils::RemoveItem(*job);
}

}  // namespace oac_tree_gui::test
