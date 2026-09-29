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

#ifndef OAC_TREE_GUI_SETTINGS_SETTINGS_MANAGER_H_
#define OAC_TREE_GUI_SETTINGS_SETTINGS_MANAGER_H_

#include <oac_tree_gui/settings/i_settings_manager.h>

#include <cstdint>
#include <map>

namespace oac_tree_gui
{

/**
 * @brief The SettingsManager class is the default implementation of ISettingsManager.
 *
 * Does not own storages. Subscribers and providers are called in the order of registration.
 * The manager must outlive every ScopedCallback it returns.
 */
class SettingsManager : public ISettingsManager
{
public:
  /**
   * @brief Sets the storage of the given scope. Nullptr removes the storage.
   */
  void SetStorage(SettingsScope scope, ISettingsStorage* storage);

  ISettingsStorage* GetStorage(SettingsScope scope) const override;

  ScopedCallback AddSubscriber(const std::string& group, SettingsScope scope,
                               settings_reader_t reader) override;

  ScopedCallback AddProvider(const std::string& group, SettingsScope scope,
                             settings_writer_t writer) override;

  void NotifyGroup(const std::string& group, SettingsScope scope) override;

  void Collect(SettingsScope scope) override;

  void NotifyAll(SettingsScope scope) override;

private:
  template <typename FuncT>
  struct Entry
  {
    std::string group;
    SettingsScope scope;
    FuncT func;
  };

  std::map<SettingsScope, ISettingsStorage*> m_storages;
  std::uint64_t m_next_id{0};
  std::map<std::uint64_t, Entry<settings_reader_t>> m_subscribers;
  std::map<std::uint64_t, Entry<settings_writer_t>> m_providers;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_SETTINGS_SETTINGS_MANAGER_H_
