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

#include "oac-tree-gui/operation/operation_action_handler.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/jobsystem/abstract_job_handler.h>
#include <oac-tree-gui/model/procedure_item.h>
#include <oac-tree-gui/model/standard_job_items.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/standarditems/container_item.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>
#include <testutils/mock_job_manager.h>
#include <testutils/mock_operation_action_context.h>
#include <testutils/mock_remote_connection_service.h>

#include <QSignalSpy>

Q_DECLARE_METATYPE(oac_tree_gui::JobItem*)

namespace oac_tree_gui
{

/**
 * @brief Tests of OperationActionHandler class using mocked dependencies.
 */
class OperationActionHandlerTest : public ::testing::Test
{
public:
  OperationActionHandlerTest()
  {
    m_procedure_container = m_model.InsertItem<mvvm::ContainerItem>();
    m_job_container = m_model.InsertItem<mvvm::ContainerItem>();
  }

  /**
   * @brief Helper function to insert new job in the container.
   */
  template <typename T>
  T* InsertJob()
  {
    return m_model.InsertItem<T>(m_job_container, mvvm::TagIndex::Append());
  }

  template <typename T>
  std::vector<T*> GetJobs()
  {
    return m_job_container->GetItems<T>(mvvm::TagIndex::kDefaultTag);
  }

  /**
   * @brief Helper function to insert new procedure in the container.
   */
  ProcedureItem* InsertProcedure()
  {
    return m_model.InsertItem<ProcedureItem>(m_procedure_container, mvvm::TagIndex::Append());
  }

  std::vector<ProcedureItem*> GetProcedures()
  {
    return m_procedure_container->GetItems<ProcedureItem>(mvvm::TagIndex::kDefaultTag);
  }

  /**
   * @brief Creates operation action handler.
   */
  std::unique_ptr<OperationActionHandler> CreateOperationHandler()
  {
    auto result = std::make_unique<OperationActionHandler>(
        &m_mock_job_manager, m_mock_operation_context.CreateContext());
    result->SetJobContainer(m_job_container);
    return result;
  }

  mvvm::ApplicationModel m_model;
  mvvm::ContainerItem* m_procedure_container{nullptr};
  mvvm::ContainerItem* m_job_container{nullptr};

  test::MockOperationActionContext m_mock_operation_context;
  test::MockRemoteConnectionService m_mock_connection_service;
  test::MockJobManager m_mock_job_manager;
};

TEST_F(OperationActionHandlerTest, AttemptToInsertWhenNoModelIsDefined)
{
  auto handler = std::make_unique<OperationActionHandler>(&m_mock_job_manager,
                                                          m_mock_operation_context.CreateContext());
  // no model is set

  ProcedureItem procedure_item;
  EXPECT_THROW(handler->SubmitLocalJob(&procedure_item), RuntimeException);
}

TEST_F(OperationActionHandlerTest, SubmitLocalJob)
{
  auto operation_handler = CreateOperationHandler();
  EXPECT_FALSE(operation_handler->SubmitLocalJob(nullptr));

  auto procedure_item = InsertProcedure();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, SubmitJob(::testing::_));

  QSignalSpy spy_selected_request(operation_handler.get(),
                                  &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_TRUE(operation_handler->SubmitLocalJob(procedure_item));

  // as a result of import request, a single LocalJobItem has been inserted into the model
  auto job_items = GetJobs<LocalJobItem>();
  ASSERT_EQ(job_items.size(), 1);

  ASSERT_EQ(job_items.at(0)->GetProcedure(), procedure_item);
  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), job_items.at(0));
}

TEST_F(OperationActionHandlerTest, SubmitThrowingLocalJob)
{
  auto operation_handler = CreateOperationHandler();
  auto procedure_item = InsertProcedure();

  ON_CALL(m_mock_job_manager, SubmitJob(::testing::_))
      .WillByDefault(::testing::Throw(RuntimeException("Submit failure")));

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob()).Times(1);
  EXPECT_CALL(m_mock_operation_context, OnMessage(::testing::_)).Times(1);
  EXPECT_CALL(m_mock_job_manager, SubmitJob(::testing::_));

  QSignalSpy spy_selected_request(operation_handler.get(),
                                  &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_FALSE(operation_handler->SubmitLocalJob(procedure_item));

  // Current implementation is that submitting throwing procedure will still lead to the appearance
  // of LocalJobItem, so the user can resubmit job later, after proper editing of bad procedure
  auto job_items = GetJobs<JobItem>();
  ASSERT_EQ(job_items.size(), 1);
  ASSERT_EQ(job_items.at(0)->GetProcedure(), procedure_item);
  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), job_items.at(0));
}

TEST_F(OperationActionHandlerTest, SubmitImportedJob)
{
  auto operation_handler = CreateOperationHandler();

  auto procedure_item = std::make_unique<ProcedureItem>();
  auto procedure_item_ptr = procedure_item.get();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, SubmitJob(::testing::_));

  QSignalSpy spy_selected_request(operation_handler.get(),
                                  &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_TRUE(operation_handler->SubmitImportedJob(std::move(procedure_item)));

  // as a result of import request, a single ImportedJobItem has been inserted into the model
  auto job_items = GetJobs<ImportedJobItem>();
  ASSERT_EQ(job_items.size(), 1);

  auto job_item = job_items.at(0);

  EXPECT_EQ(job_item->GetProcedure(), procedure_item_ptr);
  EXPECT_EQ(job_item->GetItem(ImportedJobItem::kImportedProcedure), procedure_item_ptr);
  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), job_item);
}

//! Testing import of a single remote job.
TEST_F(OperationActionHandlerTest, OnImportRemoteJobRequest)
{
  const std::string server_name("abc");
  const std::size_t job_index{42};
  const RemoteConnectionInfo connection_context{server_name, {job_index}};

  // setting up context, so it report "user choice" related to remote job import
  ON_CALL(m_mock_operation_context, OnGetRemoteConnectionInfo())
      .WillByDefault(::testing::Return(std::optional<RemoteConnectionInfo>(connection_context)));

  auto operation_handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_operation_context, OnGetRemoteConnectionInfo());
  EXPECT_CALL(m_mock_job_manager, SubmitJob(::testing::_));

  QSignalSpy spy_selected_request(operation_handler.get(),
                                  &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_TRUE(operation_handler->OnImportRemoteJobRequest());

  // as a result of import request, a single RemoteJobItem has been inserted into the model
  auto job_items = GetJobs<RemoteJobItem>();
  ASSERT_EQ(job_items.size(), 1);

  auto job_item = job_items.at(0);

  EXPECT_EQ(job_item->GetServerName(), server_name);
  EXPECT_EQ(job_item->GetRemoteJobIndex(), job_index);
  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), job_item);
}

//! Testing import of two remote jobs between two existing one.
TEST_F(OperationActionHandlerTest, ImportTwoRemoteJobs)
{
  auto job_item0 = InsertJob<LocalJobItem>();
  auto job_item1 = InsertJob<LocalJobItem>();

  const std::string server_name("abc");
  const std::size_t job_index0{42};
  const std::size_t job_index1{43};
  const RemoteConnectionInfo connection_context{server_name, {job_index0, job_index1}};

  // setting up context, so it report "user choice" related to remote job import
  ON_CALL(m_mock_operation_context, OnGetRemoteConnectionInfo())
      .WillByDefault(::testing::Return(std::optional<RemoteConnectionInfo>(connection_context)));
  // job_item0 will be always reported as selected
  ON_CALL(m_mock_operation_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item0));

  auto operation_handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob()).Times(2);
  EXPECT_CALL(m_mock_operation_context, OnGetRemoteConnectionInfo()).Times(1);
  EXPECT_CALL(m_mock_job_manager, SubmitJob(::testing::_)).Times(2);

  const QSignalSpy spy_selected_request(operation_handler.get(),
                                        &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_TRUE(operation_handler->OnImportRemoteJobRequest());

  // as a result of import request, two RemoteJobItem have been inserted into the model
  // item order: job_item0, item_inserted_second, item_inserted_fisr, job_item1
  auto inserted_job_items = GetJobs<RemoteJobItem>();

  auto item_inserted_first = inserted_job_items.at(1);
  auto item_inserted_second = inserted_job_items.at(0);
  ASSERT_EQ(inserted_job_items.size(), 2);
  EXPECT_EQ(item_inserted_first->GetServerName(), server_name);
  EXPECT_EQ(item_inserted_first->GetRemoteJobIndex(), job_index0);
  EXPECT_EQ(item_inserted_second->GetServerName(), server_name);
  EXPECT_EQ(item_inserted_second->GetRemoteJobIndex(), job_index1);

  EXPECT_EQ(spy_selected_request.count(), 2);

  // items were inserted in this order since job_item0 will always reported as selected
  const std::vector<JobItem*> expected_items(
      {job_item0, item_inserted_second, item_inserted_first, job_item1});
  EXPECT_EQ(GetJobs<JobItem>(), expected_items);
}

TEST_F(OperationActionHandlerTest, RemoveLocalJob)
{
  auto operation_handler = CreateOperationHandler();

  auto procedure_item = InsertProcedure();
  auto job_item = InsertJob<LocalJobItem>();
  job_item->SetProcedure(procedure_item);

  // job_item will be reported as selected
  ON_CALL(m_mock_operation_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item));

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, RemoveJobHandler(job_item));

  const QSignalSpy spy_selected_request(operation_handler.get(),
                                        &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_TRUE(operation_handler->OnRemoveJobRequest());

  EXPECT_EQ(spy_selected_request.count(), 0);
  EXPECT_TRUE(GetJobs<JobItem>().empty());
  EXPECT_EQ(GetProcedures(), std::vector<ProcedureItem*>({procedure_item}));
}

TEST_F(OperationActionHandlerTest, RemoveLocalJobInTheMiddle)
{
  auto operation_handler = CreateOperationHandler();

  auto job_item0 = InsertJob<LocalJobItem>();
  auto job_item1 = InsertJob<LocalJobItem>();
  auto job_item2 = InsertJob<LocalJobItem>();

  // job_item will be reported as selected
  ON_CALL(m_mock_operation_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item1));

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, RemoveJobHandler(job_item1));

  QSignalSpy spy_selected_request(operation_handler.get(),
                                  &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_TRUE(operation_handler->OnRemoveJobRequest());

  EXPECT_EQ(spy_selected_request.count(), 1);
  EXPECT_EQ(GetJobs<JobItem>(), std::vector<JobItem*>({job_item0, job_item2}));

  // last job reported as new selection
  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), job_item2);
}

TEST_F(OperationActionHandlerTest, StartPauseStepStopResetJob)
{
  LocalJobItem job_item;

  auto operation_handler = CreateOperationHandler();

  // job_item will be reported as selected
  ON_CALL(m_mock_operation_context, OnSelectedJob()).WillByDefault(::testing::Return(&job_item));

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, Start(&job_item));
  operation_handler->OnStartJobRequest();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, Pause(&job_item));
  operation_handler->OnPauseJobRequest();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, Step(&job_item));
  operation_handler->OnMakeStepRequest();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, Stop(&job_item));
  operation_handler->OnStopJobRequest();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, Reset(&job_item));
  operation_handler->OnResetJobRequest();
}

TEST_F(OperationActionHandlerTest, OnRegenerateJobRequest)
{
  LocalJobItem job_item;
  job_item.SetStatus("abc");

  auto operation_handler = CreateOperationHandler();

  QSignalSpy spy_selected_request(operation_handler.get(),
                                  &OperationActionHandler::MakeJobSelectedRequest);

  // job_item will be reported as selected
  ON_CALL(m_mock_operation_context, OnSelectedJob()).WillByDefault(::testing::Return(&job_item));

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_job_manager, RemoveJobHandler(&job_item));
  EXPECT_CALL(m_mock_job_manager, SubmitJob(&job_item));

  EXPECT_TRUE(operation_handler->OnRegenerateJobRequest());

  EXPECT_EQ(spy_selected_request.count(), 1);
  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), &job_item);
  EXPECT_EQ(GetRunnerStatus(&job_item), RunnerStatus::kUndefined);
}

//! Attempt to regenerate job when current job is still running.
TEST_F(OperationActionHandlerTest, OnRegenerateJobRequestWhenRemovalFailed)
{
  LocalJobItem job_item;
  job_item.SetStatus("abc");

  auto operation_handler = CreateOperationHandler();

  const QSignalSpy spy_selected_request(operation_handler.get(),
                                        &OperationActionHandler::MakeJobSelectedRequest);

  // job_item will be reported as selected
  ON_CALL(m_mock_operation_context, OnSelectedJob()).WillByDefault(::testing::Return(&job_item));
  ON_CALL(m_mock_job_manager, RemoveJobHandler(::testing::_))
      .WillByDefault(::testing::Throw(RuntimeException("Removal failure")));

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_operation_context, OnMessage(::testing::_));
  EXPECT_CALL(m_mock_job_manager, RemoveJobHandler(&job_item));

  EXPECT_FALSE(operation_handler->OnRegenerateJobRequest());

  EXPECT_EQ(spy_selected_request.count(), 0);
  EXPECT_EQ(job_item.GetStatus(), std::string("abc"));
}

//! Attempt to regenerate job when submit is failing.
TEST_F(OperationActionHandlerTest, OnRegenerateJobRequestWhenJobIsBroken)
{
  LocalJobItem job_item;
  job_item.SetStatus("abc");

  auto operation_handler = CreateOperationHandler();

  const QSignalSpy spy_selected_request(operation_handler.get(),
                                        &OperationActionHandler::MakeJobSelectedRequest);

  // job_item will be reported as selected
  ON_CALL(m_mock_operation_context, OnSelectedJob()).WillByDefault(::testing::Return(&job_item));
  ON_CALL(m_mock_job_manager, SubmitJob(::testing::_))
      .WillByDefault(::testing::Throw(RuntimeException("Submit failure")));

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_operation_context, OnMessage(::testing::_));
  EXPECT_CALL(m_mock_job_manager, RemoveJobHandler(&job_item));
  EXPECT_CALL(m_mock_job_manager, SubmitJob(&job_item));

  EXPECT_FALSE(operation_handler->OnRegenerateJobRequest());

  EXPECT_EQ(spy_selected_request.count(), 0);
  EXPECT_EQ(job_item.GetStatus(), std::string("abc"));
}

}  // namespace oac_tree_gui
