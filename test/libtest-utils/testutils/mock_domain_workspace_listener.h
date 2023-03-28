/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef TESTS_LIBTESTMACHINERY_MOCK_DOMAIN_WORKSPACE_LISTENER_H_
#define TESTS_LIBTESTMACHINERY_MOCK_DOMAIN_WORKSPACE_LISTENER_H_

#include <gmock/gmock.h>

#include <sup/gui/model/anyvalue_utils.h>
#include <sup/sequencer/workspace.h>

namespace testutils
{

//! Mock class to listen for domain workspace.

class MockDomainWorkspaceListener
{
public:
  MockDomainWorkspaceListener(sup::sequencer::Workspace& workspace) : m_workspace(workspace)
  {
    m_guard = m_workspace.GetCallbackGuard(this);

    auto on_variable_updated =
        [this](const std::string& name, const sup::dto::AnyValue& value, bool connected)
    {
      std::cout << sup::gui::GetAnyValueToJSONString(value) << std::endl;
      OnEvent(name, value, connected);
    };

    workspace.RegisterGenericCallback(on_variable_updated, this);
  }

  MOCK_METHOD(void, OnEvent,
              (const std::string& name, const sup::dto::AnyValue& value, bool connected));

  sup::sequencer::Workspace& m_workspace;
  sup::sequencer::ScopeGuard m_guard;
};

}  // namespace testutils

#endif  // TESTS_LIBTESTMACHINERY_MOCK_DOMAIN_WORKSPACE_LISTENER_H_
