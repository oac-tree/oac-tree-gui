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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_OPERATION_ACTION_CONTEXT_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_OPERATION_ACTION_CONTEXT_H_

#include <oac_tree_gui/jobsystem/remote_connection_info.h>
#include <oac_tree_gui/operation/operation_action_context.h>

#include <gmock/gmock.h>

namespace oac_tree_gui::test
{

/**
 * @brief The MockOperationActionContext class helps to test callbacks given to
 * OperationActionHandler via its context.
 */
class MockOperationActionContext
{
public:
  MOCK_METHOD(oac_tree_gui::JobItem*, OnSelectedJob, (), ());
  MOCK_METHOD(void, OnMessage, (const sup::gui::MessageEvent&), ());
  MOCK_METHOD(std::optional<oac_tree_gui::RemoteConnectionInfo>, OnGetRemoteConnectionInfo, (), ());

  oac_tree_gui::OperationActionContext CreateContext();
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_EVENT_LISTENER_H_
