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

#include "sequencergui/operation/operation_action_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/jobsystem/local_job_handler.h>
#include <sequencergui/jobsystem/remote_connection_service.h>
#include <sequencergui/jobsystem/remote_job_handler.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/model/standard_job_items.h>

#include <mvvm/model/application_model.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_remote_connection_service.h>
#include <testutils/standard_procedure_items.h>

using namespace sequencergui;

namespace sequencergui
{

/**
 * @brief Tests of methods from operation_action_helper.h.
 */
class OperationActionHelperTest : public ::testing::Test
{
public:
  class UnknownJobItem : public JobItem
  {
  public:
    UnknownJobItem() : JobItem("UnknownJobItem") {}
  };

  UserContext m_user_context;
  mvvm::ApplicationModel m_model;

  testutils::MockRemoteConnectionService m_mock_connection_service;
};

TEST_F(OperationActionHelperTest, CreateJobHandlerForUnknownJob)
{
  auto create_func = CreateJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  UnknownJobItem job_item;
  EXPECT_THROW(create_func(job_item), RuntimeException);
}

TEST_F(OperationActionHelperTest, CreateJobHandlerForLocalJob)
{
  // all job handlers further down requres that procedure and job are part of models
  auto procedure = testutils::CreateMessageProcedureItem("abc");
  auto job_item = CreateLocalJobItem(procedure.get());
  auto job_item_ptr = job_item.get();
  m_model.InsertItem(std::move(procedure), m_model.GetRootItem(), mvvm::TagIndex::Append());
  m_model.InsertItem(std::move(job_item), m_model.GetRootItem(), mvvm::TagIndex::Append());

  auto create_func = CreateJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  auto job_handler = create_func(*job_item_ptr);
  ASSERT_NE(dynamic_cast<LocalJobHandler*>(job_handler.get()), nullptr);
}

TEST_F(OperationActionHelperTest, CreateJobHandlerForImportedJob)
{
  // all job handlers further down requres that procedure and job are part of models
  auto job_item = CreateImportedJobItem(testutils::CreateMessageProcedureItem("abc"));
  auto job_item_ptr = job_item.get();

  m_model.InsertItem(std::move(job_item), m_model.GetRootItem(), mvvm::TagIndex::Append());

  // imported job is handled by the same LocalJobHandler
  auto create_func = CreateJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  auto job_handler = create_func(*job_item_ptr);
  ASSERT_NE(dynamic_cast<LocalJobHandler*>(job_handler.get()), nullptr);
}

TEST_F(OperationActionHelperTest, CreateJobHandlerForRemoteJob)
{
  const std::string server_name("abc");
  const size_t job_index{42};

  // all job handlers further down requres that procedure and job are part of models
  auto job_item = CreateRemoteJobItem(server_name, job_index);
  auto job_item_ptr = job_item.get();

  m_model.InsertItem(std::move(job_item), m_model.GetRootItem(), mvvm::TagIndex::Append());

  EXPECT_CALL(m_mock_connection_service, CreateJobHandler(job_item_ptr, ::testing::_)).Times(1);

  auto create_func = CreateJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  auto job_handler = create_func(*job_item_ptr);
}

}  // namespace sequencergui
