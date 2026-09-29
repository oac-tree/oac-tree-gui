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

#include "oac_tree_gui/widgets/settings_splitter.h"

#include <oac_tree_gui/settings/in_memory_settings_storage.h>
#include <oac_tree_gui/settings/settings_manager.h>

#include <gtest/gtest.h>

#include <memory>

namespace oac_tree_gui::test
{

namespace
{

constexpr auto kGroup = "View/splitter";

}  // namespace

/**
 * @brief Tests for SettingsSplitter class.
 */
class SettingsSplitterTest : public ::testing::Test
{
public:
  SettingsSplitterTest() { m_manager.SetStorage(SettingsScope::kApplication, &m_storage); }

  //! Creates splitter with two child widgets.
  std::unique_ptr<SettingsSplitter> CreateSplitter()
  {
    auto result = std::make_unique<SettingsSplitter>(kGroup, m_manager);
    result->addWidget(new QWidget);
    result->addWidget(new QWidget);
    return result;
  }

  InMemorySettingsStorage m_storage;
  SettingsManager m_manager;
};

TEST_F(SettingsSplitterTest, GetSplitterInfo)
{
  auto splitter = CreateSplitter();
  splitter->widget(0)->hide();

  const auto info = splitter->GetSplitterInfo();

  EXPECT_EQ(info.state, splitter->saveState());
  const std::vector<std::int32_t> expected_visibility({0, 1});
  EXPECT_EQ(info.visibility, expected_visibility);
}

TEST_F(SettingsSplitterTest, RestoreWithoutStoredState)
{
  auto splitter = CreateSplitter();
  const auto initial_state = splitter->saveState();

  splitter->RestoreState();

  EXPECT_EQ(splitter->saveState(), initial_state);
  EXPECT_FALSE(splitter->widget(0)->isHidden());
  EXPECT_FALSE(splitter->widget(1)->isHidden());
}

TEST_F(SettingsSplitterTest, CollectWritesState)
{
  auto splitter = CreateSplitter();
  splitter->widget(1)->hide();

  m_manager.Collect(SettingsScope::kApplication);

  const auto info = m_manager.Get<SplitterInfo>(kGroup, SettingsScope::kApplication);
  EXPECT_EQ(info.state, splitter->saveState());
  const std::vector<std::int32_t> expected_visibility({1, 0});
  EXPECT_EQ(info.visibility, expected_visibility);
}

TEST_F(SettingsSplitterTest, RoundTrip)
{
  auto splitter = CreateSplitter();
  splitter->setSizes({100, 300});
  splitter->widget(0)->hide();
  const auto state = splitter->saveState();

  m_manager.Collect(SettingsScope::kApplication);
  splitter.reset();

  auto new_splitter = CreateSplitter();
  new_splitter->RestoreState();

  EXPECT_EQ(new_splitter->saveState(), state);
  EXPECT_TRUE(new_splitter->widget(0)->isHidden());
  EXPECT_FALSE(new_splitter->widget(1)->isHidden());
}

TEST_F(SettingsSplitterTest, VisibilityOfDifferentWidgetCountIsIgnored)
{
  m_manager.Set(kGroup, SplitterInfo{{}, {0, 0, 0}}, SettingsScope::kApplication);

  auto splitter = CreateSplitter();
  splitter->RestoreState();

  EXPECT_FALSE(splitter->widget(0)->isHidden());
  EXPECT_FALSE(splitter->widget(1)->isHidden());
}

TEST_F(SettingsSplitterTest, DestroyedSplitterIsNotCollected)
{
  CreateSplitter().reset();

  m_manager.Collect(SettingsScope::kApplication);

  EXPECT_TRUE(m_storage.GetKeys().empty());
}

}  // namespace oac_tree_gui::test
