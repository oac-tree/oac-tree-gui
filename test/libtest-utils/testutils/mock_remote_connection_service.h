/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_REMOTE_CONNECTION_SERVICE_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_REMOTE_CONNECTION_SERVICE_H_

#include <sequencergui/jobsystem/i_remote_connection_service.h>

#include <gmock/gmock.h>
namespace sequencergui
{
class IAutomationClient;
class AbstractJobHandler;
struct UserContext;
class RemoteJobItem;
}  // namespace sequencergui

namespace testutils
{

/**
 * @brief The MockRemoteConnectionService class is intended for usage *instead* of
 * RemoteConnectionService..
 */
class MockRemoteConnectionService : public sequencergui::IRemoteConnectionService
{
public:
  MOCK_METHOD(bool, Connect, (const std::string& server_name), (override));
  MOCK_METHOD(void, Disconnect, (const std::string& server_name), (override));
  MOCK_METHOD(bool, IsConnected, (const std::string& server_name), (const, override));
  MOCK_METHOD(bool, HasClient, (const std::string& server_name), (const, override));
  MOCK_METHOD(std::vector<std::string>, GetServerNames, (), (const, override));
  MOCK_METHOD(sequencergui::IAutomationClient&, GetAutomationClient,
              (const std::string& server_name), (override));
  MOCK_METHOD(std::unique_ptr<sequencergui::AbstractJobHandler>, CreateJobHandler,
              (sequencergui::RemoteJobItem*, const sequencergui::UserContext&), (override));
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_REMOTE_CONNECTION_SERVICE_H_
