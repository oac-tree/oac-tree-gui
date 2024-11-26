/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "sequencergui/operation/operation_action_handler.h"

#include <sequencergui/jobsystem/abstract_job_handler.h>
#include <sequencergui/model/job_model.h>
#include <sequencergui/model/standard_job_items.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/test/test_helper.h>

#include <gtest/gtest.h>
#include <testutils/mock_job_manager.h>
#include <testutils/mock_operation_action_context.h>
#include <testutils/mock_remote_connection_service.h>

#include <QSignalSpy>

Q_DECLARE_METATYPE(sequencergui::JobItem*)

namespace sequencergui
{

using msec = std::chrono::milliseconds;

/**
 * @brief Tests of OperationActionHandler class using mocked depencies.
 */
class OperationActionHandlerTest : public ::testing::Test
{
public:
  /**
   * @brief Creates operation action handler.
   */
  std::unique_ptr<OperationActionHandler> CreateOperationHandler()
  {
    auto result = std::make_unique<OperationActionHandler>(
        &m_mock_mock_job_manager, m_mock_operation_context.CreateContext());
    result->SetJobModel(&m_model);
    return result;
  }

  JobModel m_model;
  testutils::MockOperationActionContext m_mock_operation_context;
  testutils::MockRemoteConnectionService m_mock_connection_service;
  testutils::MockJobManager m_mock_mock_job_manager;
};

//! Testing import of a single remote job.
TEST_F(OperationActionHandlerTest, OnImportRemoteJobRequest)
{
  const std::string server_name("abc");
  const size_t job_index{42};
  const RemoteConnectionContext connection_context{server_name, {job_index}};

  // setting up context, so it report "user choice" related to remote job import
  ON_CALL(m_mock_operation_context, OnGetRemoteConnectionContext())
      .WillByDefault(::testing::Return(std::optional<RemoteConnectionContext>(connection_context)));

  auto operation_handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob());
  EXPECT_CALL(m_mock_operation_context, OnGetRemoteConnectionContext());
  EXPECT_CALL(m_mock_mock_job_manager, SubmitJob(::testing::_));

  QSignalSpy spy_selected_request(operation_handler.get(),
                                  &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_TRUE(operation_handler->OnImportRemoteJobRequest());

  // as a result of import request, a single RemoteJobItem has been inserted into the model
  auto job_item = dynamic_cast<RemoteJobItem*>(mvvm::utils::GetTopItem(&m_model));
  ASSERT_NE(job_item, nullptr);
  EXPECT_EQ(job_item->GetServerName(), server_name);
  EXPECT_EQ(job_item->GetRemoteJobIndex(), job_index);

  EXPECT_EQ(mvvm::test::GetSendItem<JobItem*>(spy_selected_request), job_item);
}

//! Testing import of two remote jobs between two existing one.
TEST_F(OperationActionHandlerTest, ImportOfTwoRemoteJobs)
{
  auto job_item0 = m_model.InsertItem<LocalJobItem>();
  auto job_item1 = m_model.InsertItem<LocalJobItem>();

  const std::string server_name("abc");
  const size_t job_index0{42};
  const size_t job_index1{43};
  const RemoteConnectionContext connection_context{server_name, {job_index0, job_index1}};

  // setting up context, so it report "user choice" related to remote job import
  ON_CALL(m_mock_operation_context, OnGetRemoteConnectionContext())
      .WillByDefault(::testing::Return(std::optional<RemoteConnectionContext>(connection_context)));
  // job_item0 will be always reported as selected
  ON_CALL(m_mock_operation_context, OnSelectedJob()).WillByDefault(::testing::Return(job_item0));

  auto operation_handler = CreateOperationHandler();

  EXPECT_CALL(m_mock_operation_context, OnSelectedJob()).Times(2);
  EXPECT_CALL(m_mock_operation_context, OnGetRemoteConnectionContext()).Times(1);
  EXPECT_CALL(m_mock_mock_job_manager, SubmitJob(::testing::_)).Times(2);

  QSignalSpy spy_selected_request(operation_handler.get(),
                                  &OperationActionHandler::MakeJobSelectedRequest);

  EXPECT_TRUE(operation_handler->OnImportRemoteJobRequest());

  // as a result of import request, two RemoteJobItem have been inserted into the model
  // item order: job_item0, item_inserted_second, item_inserted_fisr, job_item1
  auto inserted_job_items = mvvm::utils::GetTopItems<RemoteJobItem>(&m_model);
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
  EXPECT_EQ(mvvm::utils::GetTopItems<JobItem>(&m_model), expected_items);
}

}  // namespace sequencergui
