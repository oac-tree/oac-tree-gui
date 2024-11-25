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

#include <gtest/gtest.h>
#include <testutils/mock_job_manager.h>
#include <testutils/mock_operation_action_context.h>
#include <testutils/mock_remote_connection_service.h>

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

  EXPECT_TRUE(operation_handler->OnImportRemoteJobRequest());

  // as a result of import request, a single RemoteJobItem has been inserted into the model
  auto job_item = dynamic_cast<RemoteJobItem*>(mvvm::utils::GetTopItem(&m_model));
  ASSERT_NE(job_item, nullptr);
  EXPECT_EQ(job_item->GetServerName(), server_name);
  EXPECT_EQ(job_item->GetRemoteJobIndex(), job_index);
}

}  // namespace sequencergui
