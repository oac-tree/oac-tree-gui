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

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/domain/sequencer_types_fwd.h>

#include <sup/dto/anyvalue.h>
#include <sup/sequencer/workspace.h>

#include <gtest/gtest.h>
#include <testutils/mock_domain_workspace_listener.h>
#include <testutils/test_utils.h>

using ::testing::_;
using namespace sequencergui;

//! Testing domain sequencer workspace for notifications accompanying Setup, and other corner cases.

class SequencerWorkspaceCornerCaseTests : public ::testing::Test
{
public:
  static std::unique_ptr<variable_t> CreateLocalVariable(const std::string& name,
                                                  const sup::dto::AnyValue& initial_value)
  {
    auto local_variable = CreateDomainVariable(domainconstants::kLocalVariableType);
    local_variable->SetName(name);
    local_variable->AddAttribute("type", sup::gui::AnyTypeToJSONString(initial_value));
    local_variable->AddAttribute("value", sup::gui::ValuesToJSONString(initial_value));
    return local_variable;
  }

  sup::sequencer::Workspace m_workspace;
};

//! Single variable in workspace.
//! Test prouves that workspace->Setup doesn't lead to the notification about initial value.

TEST_F(SequencerWorkspaceCornerCaseTests, LocalVariable)
{
  const std::string var_name("var0");

  // creating local variable
  sup::dto::AnyValue initial_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 42});
  auto local_variable = CreateLocalVariable("abc", initial_value);
  auto local_variable_ptr = local_variable.get();

  testutils::MockDomainWorkspaceListener listener(m_workspace);

  // adding variable doesn't cause notifications
  EXPECT_CALL(listener, OnEvent(_, _, _)).Times(0);
  m_workspace.AddVariable(var_name, local_variable.release());

  EXPECT_EQ(local_variable_ptr, m_workspace.GetVariable(var_name));
  EXPECT_FALSE(local_variable_ptr->IsAvailable());

  // workspace setup doesn't cause notifications
  EXPECT_CALL(listener, OnEvent(_, _, _)).Times(0);
  EXPECT_NO_THROW(m_workspace.Setup());

  // variable is available and has correct value
  EXPECT_TRUE(local_variable_ptr->IsAvailable());
  sup::dto::AnyValue current_value;
  EXPECT_TRUE(local_variable_ptr->GetValue(current_value));
  EXPECT_EQ(current_value, initial_value);

  // expecting notificarion on new value set
  sup::dto::AnyValue new_value(sup::dto::AnyValue{sup::dto::SignedInteger32Type, 44});
  EXPECT_CALL(listener, OnEvent(var_name, new_value, true)).Times(1);

  local_variable_ptr->SetValue(new_value);
}
