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

#ifndef OAC_TREE_GUI_SETTINGS_I_SETTINGS_MANAGER_H_
#define OAC_TREE_GUI_SETTINGS_I_SETTINGS_MANAGER_H_

#include <oac_tree_gui/settings/scoped_callback.h>
#include <oac_tree_gui/settings/settings_archive.h>

#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>

namespace oac_tree_gui
{

/**
 * @brief The SettingsScope enum defines where settings are stored.
 */
enum class SettingsScope : std::uint8_t
{
  kApplication,  //!< persistent storage, same for all projects
  kProject       //!< project file
};

/**
 * @brief The ISettingsManager class is an interface to read, write and observe widget settings.
 *
 * Settings are structs with Visit(Archive&, T&) function defined.
 * Will be stored under a group name in the storage of the given scope.
 *
 * The manager must outlive every ScopedCallback it returns.
 */
class ISettingsManager
{
public:
  //! A function reading settings of the group from the storage.
  using settings_reader_t =
      std::function<void(const ISettingsStorage& storage, const std::string& group)>;

  //! A function writing settings of the group into the storage.
  using settings_writer_t =
      std::function<void(ISettingsStorage& storage, const std::string& group)>;

  ISettingsManager() = default;
  virtual ~ISettingsManager() = default;

  ISettingsManager(const ISettingsManager&) = delete;
  ISettingsManager& operator=(const ISettingsManager&) = delete;
  ISettingsManager(ISettingsManager&&) = delete;
  ISettingsManager& operator=(ISettingsManager&&) = delete;

  /**
   * @brief Returns the storage of the given scope, or nullptr if no storage is set.
   */
  virtual ISettingsStorage* GetStorage(SettingsScope scope) const = 0;

  /**
   * @brief Adds a subscriber of the group.
   *
   * The reading function will be called on every change of option registered under group name.
   *
   * @return The object removing the subscriber on destruction.
   */
  virtual ScopedCallback AddSubscriber(const std::string& group, SettingsScope scope,
                                       settings_reader_t reader) = 0;

  /**
   * @brief Adds a provider of the group.
   *
   * The manager will call provided function to get the value from provider and write to the
   * storage.
   *
   * @return The object removing the provider on destruction.
   */
  virtual ScopedCallback AddProvider(const std::string& group, SettingsScope scope,
                                     settings_writer_t writer) = 0;

  /**
   * @brief Calls subscribers of the group. Does nothing if no storage is set.
   */
  virtual void NotifyGroup(const std::string& group, SettingsScope scope) = 0;

  /**
   * @brief Calls every provider of the scope and writes the results. Does not notify.
   *
   * Does nothing if no storage is set.
   */
  virtual void Collect(SettingsScope scope) = 0;

  /**
   * @brief Calls every subscriber of the scope with the stored value.
   *
   * Does nothing if no storage is set.
   */
  virtual void NotifyAll(SettingsScope scope) = 0;

  /**
   * @brief Reads the settings struct of the group.
   *
   * @param group The group of the struct.
   * @param scope The scope of the storage.
   * @param initial The initial value. Fields missing in the storage keep it.
   * @return The struct.
   * @throw RuntimeException if no storage is set.
   */
  template <typename T>
  T Get(const std::string& group, SettingsScope scope, T initial = T{}) const;

  /**
   * @brief Writes the settings struct of the group and calls its subscribers.
   *
   * @throw RuntimeException if no storage is set.
   */
  template <typename T>
  void Set(const std::string& group, const T& value, SettingsScope scope);

  /**
   * @brief Subscribes to changes of the group.
   *
   * The callback receives the stored struct on Set and NotifyAll.
   *
   * @return The object removing the subscriber on destruction.
   */
  template <typename T>
  ScopedCallback Subscribe(const std::string& group, SettingsScope scope,
                           std::function<void(const T&)> callback);

  /**
   * @brief Registers a provider of the group.
   *
   * The provider returns the current settings struct. It is called on Collect.
   *
   * @return The object removing the provider on destruction.
   */
  template <typename Func>
  ScopedCallback RegisterProvider(const std::string& group, SettingsScope scope, Func provider);

private:
  //! Returns the storage of the given scope. Throws if no storage is set.
  ISettingsStorage& GetValidStorage(SettingsScope scope) const;
};

template <typename T>
inline T ISettingsManager::Get(const std::string& group, SettingsScope scope, T initial) const
{
  return ReadFromStorage<T>(GetValidStorage(scope), group, std::move(initial));
}

template <typename T>
inline void ISettingsManager::Set(const std::string& group, const T& value, SettingsScope scope)
{
  WriteToStorage(GetValidStorage(scope), group, value);
  NotifyGroup(group, scope);
}

template <typename T>
inline ScopedCallback ISettingsManager::Subscribe(const std::string& group, SettingsScope scope,
                                                  std::function<void(const T&)> callback)
{
  auto reader = [callback = std::move(callback)](const ISettingsStorage& storage,
                                                 const std::string& group_name)
  { callback(ReadFromStorage<T>(storage, group_name)); };
  return AddSubscriber(group, scope, std::move(reader));
}

template <typename Func>
inline ScopedCallback ISettingsManager::RegisterProvider(const std::string& group,
                                                         SettingsScope scope, Func provider)
{
  using value_t = std::decay_t<std::invoke_result_t<Func&>>;
  auto writer = [provider = std::move(provider)](ISettingsStorage& storage,
                                                 const std::string& group_name) mutable
  {
    auto provided_value = provider();  // call
    WriteToStorage<value_t>(storage, group_name, provided_value);
  };
  return AddProvider(group, scope, std::move(writer));
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_SETTINGS_I_SETTINGS_MANAGER_H_
