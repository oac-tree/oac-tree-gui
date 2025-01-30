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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_WORKSPACE_LISTENER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_WORKSPACE_LISTENER_H_

#include <sup/gui/model/anyvalue_utils.h>

#include <sup/oac-tree/workspace.h>

#include <gmock/gmock.h>

namespace oac_tree_gui::test
{

/**
 * @brief The MockDomainWorkspaceListener class listens for domain workspace notifications.
 */
class MockDomainWorkspaceListener
{
public:
  explicit MockDomainWorkspaceListener(sup::oac_tree::Workspace& workspace)
      : m_workspace(workspace)
  {
    m_guard = m_workspace.GetCallbackGuard(this);

    auto on_variable_updated = [this](const std::string& name, const sup::dto::AnyValue& value,
                                      bool connected) { OnEvent(name, value, connected); };

    workspace.RegisterGenericCallback(on_variable_updated, this);
  }

  MOCK_METHOD(void, OnEvent,
              (const std::string& name, const sup::dto::AnyValue& value, bool connected));

  sup::oac_tree::Workspace& m_workspace;
  sup::oac_tree::ScopeGuard m_guard;
};

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_MOCK_DOMAIN_WORKSPACE_LISTENER_H_
