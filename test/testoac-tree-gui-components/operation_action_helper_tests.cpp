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

#include "oac_tree_gui/operation/operation_action_helper.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/jobsystem/local_job_handler.h>
#include <oac_tree_gui/jobsystem/remote_connection_service.h>
#include <oac_tree_gui/jobsystem/remote_job_handler.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/standard_job_items.h>

#include <mvvm/model/application_model.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <testutils/mock_remote_connection_service.h>
#include <testutils/standard_procedure_items.h>
#include <testutils/folder_test.h>

namespace oac_tree_gui
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

  test::MockRemoteConnectionService m_mock_connection_service;
};

TEST_F(OperationActionHelperTest, CreateJobHandlerForUnknownJob)
{
  auto create_func = GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  UnknownJobItem job_item;
  EXPECT_THROW(create_func(job_item), RuntimeException);
}

TEST_F(OperationActionHelperTest, CreateJobHandlerForLocalJob)
{
  // all job handlers further down requres that procedure and job are part of models
  auto procedure = test::CreateMessageProcedureItem("abc");
  auto job_item = CreateLocalJobItem(procedure.get());
  auto job_item_ptr = job_item.get();
  m_model.InsertItem(std::move(procedure), m_model.GetRootItem(), mvvm::TagIndex::Append());
  m_model.InsertItem(std::move(job_item), m_model.GetRootItem(), mvvm::TagIndex::Append());

  auto create_func = GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  auto job_handler = create_func(*job_item_ptr);
  ASSERT_NE(dynamic_cast<LocalJobHandler*>(job_handler.get()), nullptr);
}

TEST_F(OperationActionHelperTest, CreateJobHandlerForImportedJob)
{
  // all job handlers further down requres that procedure and job are part of models
  auto job_item = CreateImportedJobItem(test::CreateMessageProcedureItem("abc"));
  auto job_item_ptr = job_item.get();

  m_model.InsertItem(std::move(job_item), m_model.GetRootItem(), mvvm::TagIndex::Append());

  // imported job is handled by the same LocalJobHandler
  auto create_func = GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  auto job_handler = create_func(*job_item_ptr);
  ASSERT_NE(dynamic_cast<LocalJobHandler*>(job_handler.get()), nullptr);
}

TEST_F(OperationActionHelperTest, CreateJobHandlerForRemoteJob)
{
  const std::string server_name("abc");
  const std::size_t job_index{42};

  // all job handlers further down requres that procedure and job are part of models
  auto job_item = CreateRemoteJobItem(server_name, job_index);
  auto job_item_ptr = job_item.get();

  m_model.InsertItem(std::move(job_item), m_model.GetRootItem(), mvvm::TagIndex::Append());

  EXPECT_CALL(m_mock_connection_service, CreateJobHandler(job_item_ptr, ::testing::_)).Times(1);

  auto create_func = GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  auto job_handler = create_func(*job_item_ptr);
}

TEST_F(OperationActionHelperTest, CreateJobHandlerForFileBasedJobWhenFileIsAbsent)
{
  // all job handlers further down requres that procedure and job are part of models
  auto job_item = CreateFileBasedJobItem("no-such-file");
  auto job_item_ptr = job_item.get();
  m_model.InsertItem(std::move(job_item), m_model.GetRootItem(), mvvm::TagIndex::Append());

  auto create_func = GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);

  // throw from sup::xml about absent file, can't check since xml/exceptions.h are not exposed
  EXPECT_ANY_THROW(create_func(*job_item_ptr));
}

TEST_F(OperationActionHelperTest, CreateJobHandlerForFileBasedJob)
{
  // all job handlers further down requres that procedure and job are part of models
  auto job_item = CreateFileBasedJobItem(test::ProjectResourceDir()+"/fallback.xml");
  auto job_item_ptr = job_item.get();
  m_model.InsertItem(std::move(job_item), m_model.GetRootItem(), mvvm::TagIndex::Append());

  auto create_func = GetJobHandlerFactoryFunc(m_user_context, m_mock_connection_service);
  auto job_handler = create_func(*job_item_ptr);
  ASSERT_NE(dynamic_cast<LocalJobHandler*>(job_handler.get()), nullptr);
}

}  // namespace oac_tree_gui
