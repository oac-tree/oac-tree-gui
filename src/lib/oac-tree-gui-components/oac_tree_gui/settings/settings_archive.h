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

#ifndef OAC_TREE_GUI_SETTINGS_SETTINGS_ARCHIVE_H_
#define OAC_TREE_GUI_SETTINGS_SETTINGS_ARCHIVE_H_

#include <oac_tree_gui/settings/i_settings_storage.h>

#include <QByteArray>
#include <string>
#include <type_traits>
#include <vector>

namespace oac_tree_gui
{

namespace detail
{

//! Checks if Visit(archive, value) is defined for the given archive and value types.
template <typename Archive, typename T, typename = void>
struct has_visit : std::false_type
{
};

template <typename Archive, typename T>
struct has_visit<Archive, T,
                 std::void_t<decltype(Visit(std::declval<Archive&>(), std::declval<T&>()))>>
    : std::true_type
{
};

//! Checks if the type is std::vector.
template <typename T>
struct is_vector : std::false_type
{
};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type
{
};

//! Checks if the type is stored directly as a single storage value.
template <typename T>
inline constexpr bool is_settings_value_v =
    std::is_same_v<T, bool> || std::is_same_v<T, mvvm::int32> || std::is_same_v<T, double>
    || std::is_same_v<T, std::string> || std::is_same_v<T, QByteArray> || std::is_enum_v<T>;

}  // namespace detail

/**
 * @brief Joins group and name into a storage key "group/name".
 *
 * Returns the name alone if the group is empty.
 */
std::string JoinSettingsKey(const std::string& group, const std::string& name);

/**
 * @brief Converts a field value to a storage value.
 */
template <typename T>
mvvm::variant_t ToSettingsValue(const T& value)
{
  return mvvm::variant_t{value};
}

/**
 * @brief Converts QByteArray to a storage value holding base64 string.
 */
mvvm::variant_t ToSettingsValue(const QByteArray& value);

/**
 * @brief Assigns a storage value to a field value.
 *
 * A string storage value is parsed for bool, int32 and double. Storages losing the type, such as
 * QSettings, return strings.
 *
 * @return True if the value was assigned. False if the storage value is absent, has another type,
 * or cannot be parsed.
 */
bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, bool& value);
bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, mvvm::int32& value);
bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, double& value);
bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, std::string& value);
bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, QByteArray& value);

/**
 * @brief The WriteArchive class writes struct fields into a settings storage.
 *
 * Each field goes under "group/name". Nested structs go under "group/name/...". Vectors are
 * stored as "group/name/size" and "group/name/<index>". Enums are stored as int32.
 */
class WriteArchive
{
public:
  WriteArchive(ISettingsStorage& storage, std::string group);

  /**
   * @brief Writes the field with the given name.
   */
  template <typename T>
  void operator()(const std::string& name, T& value);

private:
  ISettingsStorage& m_storage;
  std::string m_group;
};

/**
 * @brief The ReadArchive class reads struct fields from a settings storage.
 *
 * Uses the same layout as WriteArchive. A missing key, or a value of another type, keeps the
 * field unchanged.
 */
class ReadArchive
{
public:
  ReadArchive(const ISettingsStorage& storage, std::string group);

  /**
   * @brief Reads the field with the given name.
   */
  template <typename T>
  void operator()(const std::string& name, T& value);

private:
  const ISettingsStorage& m_storage;
  std::string m_group;
};

/**
 * @brief Writes the struct into the storage under the given group.
 *
 * The struct must provide Visit(Archive&, T&).
 */
template <typename T>
void WriteToStorage(ISettingsStorage& storage, const std::string& group, const T& value);

/**
 * @brief Reads the struct from the storage under the given group.
 *
 * @param storage The storage to read from.
 * @param group The group of the struct.
 * @param value The initial value. Fields missing in the storage keep it.
 * @return The struct.
 */
template <typename T>
T ReadFromStorage(const ISettingsStorage& storage, const std::string& group, T value = T{});

//! Name of the key holding the size of a vector.
constexpr auto kSettingsVectorSizeKey = "size";

template <typename T>
inline void WriteArchive::operator()(const std::string& name, T& value)
{
  const auto key = JoinSettingsKey(m_group, name);

  if constexpr (detail::is_vector<T>::value)
  {
    static_assert(!std::is_same_v<T, std::vector<bool>>, "std::vector<bool> is not supported");
    m_storage.Write(JoinSettingsKey(key, kSettingsVectorSizeKey),
                    ToSettingsValue(static_cast<mvvm::int32>(value.size())));
    WriteArchive element_archive(m_storage, key);
    for (std::size_t index = 0; index < value.size(); ++index)
    {
      element_archive(std::to_string(index), value[index]);
    }
  }
  else if constexpr (detail::has_visit<WriteArchive, T>::value)
  {
    WriteArchive nested_archive(m_storage, key);
    Visit(nested_archive, value);
  }
  else if constexpr (std::is_enum_v<T>)
  {
    m_storage.Write(key, ToSettingsValue(static_cast<mvvm::int32>(value)));
  }
  else
  {
    static_assert(detail::is_settings_value_v<T>, "Unsupported settings field type");
    m_storage.Write(key, ToSettingsValue(value));
  }
}

template <typename T>
inline void ReadArchive::operator()(const std::string& name, T& value)
{
  const auto key = JoinSettingsKey(m_group, name);

  if constexpr (detail::is_vector<T>::value)
  {
    static_assert(!std::is_same_v<T, std::vector<bool>>, "std::vector<bool> is not supported");
    mvvm::int32 size{0};
    if (!FromSettingsValue(m_storage.Read(JoinSettingsKey(key, kSettingsVectorSizeKey)), size)
        || size < 0)
    {
      return;
    }
    T result(static_cast<std::size_t>(size));
    ReadArchive element_archive(m_storage, key);
    for (std::size_t index = 0; index < result.size(); ++index)
    {
      element_archive(std::to_string(index), result[index]);
    }
    value = std::move(result);
  }
  else if constexpr (detail::has_visit<ReadArchive, T>::value)
  {
    ReadArchive nested_archive(m_storage, key);
    Visit(nested_archive, value);
  }
  else if constexpr (std::is_enum_v<T>)
  {
    mvvm::int32 underlying_value{0};
    if (FromSettingsValue(m_storage.Read(key), underlying_value))
    {
      value = static_cast<T>(underlying_value);
    }
  }
  else
  {
    static_assert(detail::is_settings_value_v<T>, "Unsupported settings field type");
    (void)FromSettingsValue(m_storage.Read(key), value);
  }
}

template <typename T>
inline void WriteToStorage(ISettingsStorage& storage, const std::string& group, const T& value)
{
  T copy(value);
  WriteArchive archive(storage, group);
  Visit(archive, copy);
}

template <typename T>
inline T ReadFromStorage(const ISettingsStorage& storage, const std::string& group, T value)
{
  ReadArchive archive(storage, group);
  Visit(archive, value);
  return value;
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_SETTINGS_SETTINGS_ARCHIVE_H_
