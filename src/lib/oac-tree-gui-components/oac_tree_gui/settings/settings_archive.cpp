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

#include "settings_archive.h"

namespace oac_tree_gui
{

namespace
{

constexpr auto kGroupSeparator = "/";

/**
 * @brief Assigns the storage value if it holds T, or a string convertible to T.
 *
 * The @p settings_value can be either a variant, based on the same type as @p value, or its string
 * representation. The function is necessary since QSettings storage loses type information, and
 * gives back everything as QVariant(QString)
 *
 * @param settings_value The value obtained after reading settings storage.
 * @param value The real value which shall be updated.
 * @return True in the case of success
 */
template <typename T>
bool AssignFromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, T& value)
{
  if (!settings_value.has_value())
  {
    return false;
  }

  // settings_value contains the same type as we need
  if (const auto stored = std::get_if<T>(&settings_value.value()); stored != nullptr)
  {
    value = *stored;
    return true;
  }

  // Settings_value contains a string. We will use our serialization, to encode it into value.
  const auto str = std::get_if<std::string>(&settings_value.value());
  if (str == nullptr)
  {
    return false;
  }
  try
  {
    const auto type_code = mvvm::GetTypeCode(mvvm::variant_t{value});
    value = std::get<T>(mvvm::utils::ValueFromString(type_code, *str));
    return true;
  }
  catch (const std::exception&)
  {
    return false;
  }
}

}  // namespace

std::string JoinSettingsKey(const std::string& group, const std::string& name)
{
  return group.empty() ? name : group + kGroupSeparator + name;
}

mvvm::variant_t ToSettingsValue(const QByteArray& value)
{
  return mvvm::variant_t{value.toBase64().toStdString()};
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, bool& value)
{
  return AssignFromSettingsValue(settings_value, value);
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, mvvm::int32& value)
{
  return AssignFromSettingsValue(settings_value, value);
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, double& value)
{
  return AssignFromSettingsValue(settings_value, value);
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, std::string& value)
{
  return AssignFromSettingsValue(settings_value, value);
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, QByteArray& value)
{
  std::string base64;
  if (!AssignFromSettingsValue(settings_value, base64))
  {
    return false;
  }
  value = QByteArray::fromBase64(QByteArray::fromStdString(base64));
  return true;
}

WriteArchive::WriteArchive(ISettingsStorage& storage, std::string group)
    : m_storage(storage), m_group(std::move(group))
{
}

ReadArchive::ReadArchive(const ISettingsStorage& storage, std::string group)
    : m_storage(storage), m_group(std::move(group))
{
}

}  // namespace oac_tree_gui
