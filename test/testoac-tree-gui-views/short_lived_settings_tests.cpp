/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include <oac_tree_gui/jobsystem/objects/abstract_job_handler.h>
#include <oac_tree_gui/views/editors/anyvalue_compact_tree_editor.h>
#include <oac_tree_gui/views/operation/remote_connection_dialog.h>
#include <oac_tree_gui/views/operation/server_settings_widget.h>

#include <gtest/gtest.h>
#include <testutils/mock_remote_connection_service.h>
#include <testutils/test_settings_manager.h>

#include <QLineEdit>

namespace oac_tree_gui::test
{

/**
 * @brief Tests for short-lived widgets storing settings in the settings manager.
 */
class ShortLivedSettingsTest : public ::testing::Test
{
public:
  TestSettingsManager m_settings_manager;
};

TEST_F(ShortLivedSettingsTest, RemoteConnectionDialogRoundTrip)
{
  ::testing::NiceMock<MockRemoteConnectionService> connection_service;
  {
    RemoteConnectionDialog dialog(&connection_service, m_settings_manager);
    dialog.resize(321, 234);
    dialog.findChild<ServerSettingsWidget*>()->SetServerType(AutomationServerType::kEPICS);
    dialog.findChild<QLineEdit*>(ServerSettingsWidget::kServerNameLineEditName)
        ->setText("MyEPICSServer");
    dialog.done(QDialog::Rejected);
  }

  const RemoteConnectionDialog dialog(&connection_service, m_settings_manager);
  EXPECT_EQ(dialog.size(), QSize(321, 234));
  const auto info = dialog.findChild<ServerSettingsWidget*>()->GetServerSettingsInfo();
  EXPECT_EQ(info.server_type, AutomationServerType::kEPICS);
  EXPECT_EQ(info.server_name, "MyEPICSServer");
}

TEST_F(ShortLivedSettingsTest, AnyValueCompactTreeEditorCollect)
{
  const AnyValueCompactTreeEditor editor(m_settings_manager);

  m_settings_manager.Collect(sup::gui::SettingsScope::kApplication);

  EXPECT_TRUE(m_settings_manager.GetStorage(sup::gui::SettingsScope::kApplication)
                  ->Read("AnyValueCompactTreeEditor/tree_header/state")
                  .has_value());
}

}  // namespace oac_tree_gui::test
