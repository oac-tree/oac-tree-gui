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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_REMOTE_CONNECTION_SERVICE_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_REMOTE_CONNECTION_SERVICE_H_

#include <oac_tree_gui/jobsystem/i_remote_connection_service.h>

#include <gmock/gmock.h>

namespace oac_tree_gui
{

class IAutomationClient;
class AbstractJobHandler;
struct UserContext;
class RemoteJobItem;

namespace test
{

/**
 * @brief The MockRemoteConnectionService class is intended for usage *instead* of
 * RemoteConnectionService..
 */
class MockRemoteConnectionService : public oac_tree_gui::IRemoteConnectionService
{
public:
  MOCK_METHOD(bool, Connect, (const std::string& server_name), (override));
  MOCK_METHOD(void, Disconnect, (const std::string& server_name), (override));
  MOCK_METHOD(bool, IsConnected, (const std::string& server_name), (const, override));
  MOCK_METHOD(bool, HasClient, (const std::string& server_name), (const, override));
  MOCK_METHOD(std::vector<std::string>, GetServerNames, (), (const, override));
  MOCK_METHOD(oac_tree_gui::IAutomationClient&, GetAutomationClient,
              (const std::string& server_name), (override));
  MOCK_METHOD(std::unique_ptr<oac_tree_gui::AbstractJobHandler>, CreateJobHandler,
              (oac_tree_gui::RemoteJobItem*, const oac_tree_gui::UserContext&), (override));
};

}  // namespace test

}  // namespace oac_tree_gui

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_REMOTE_CONNECTION_SERVICE_H_
