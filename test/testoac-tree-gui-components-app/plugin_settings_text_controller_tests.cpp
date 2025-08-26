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

#include "oac_tree_gui/components/plugin_settings_text_controller.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/model/plugin_settings_item.h>
#include <oac_tree_gui/model/sequencer_settings_constants.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QCheckBox>
#include <QTextEdit>

namespace oac_tree_gui::test
{

class PluginSettingsTextControllerTest : public ::testing::Test
{
public:
  PluginSettingsTextControllerTest()
  {
    m_settings_item = m_model.InsertItem<PluginSettingsItem>();
  }

  /**
   * @brief Creates a controller which is looking to properties of "plugins directory list".
   */
  std::unique_ptr<PluginSettingsTextController> CreateController()
  {
    const TextControllerContext context{constants::kUsePluginDirList, &m_check_box,
                                  constants::kPluginDirList, &m_text_edit};
    return std::make_unique<PluginSettingsTextController>(context);
  }

  QCheckBox m_check_box;
  QTextEdit m_text_edit;
  mvvm::ApplicationModel m_model;
  PluginSettingsItem* m_settings_item{nullptr};
};

TEST_F(PluginSettingsTextControllerTest, ConstructFailure)
{
  EXPECT_THROW(PluginSettingsTextController({}), RuntimeException);
  EXPECT_THROW(PluginSettingsTextController({"", nullptr, "", nullptr}), RuntimeException);
  EXPECT_THROW(PluginSettingsTextController({"", &m_check_box, "", nullptr}), RuntimeException);
}

TEST_F(PluginSettingsTextControllerTest, InitialStateWhenEverythingIsEnabledAndEmpty)
{
  auto controller = CreateController();
  m_settings_item->SetUsePluginDirList(true);
  m_settings_item->SetPluginDirList({});
  controller->SetItem(m_settings_item);

  EXPECT_EQ(controller->GetItem(), m_settings_item);
  EXPECT_TRUE(m_check_box.isChecked());
  EXPECT_TRUE(m_text_edit.isEnabled());
  EXPECT_EQ(m_text_edit.toPlainText(),QString());
}

}  // namespace oac_tree_gui::test
