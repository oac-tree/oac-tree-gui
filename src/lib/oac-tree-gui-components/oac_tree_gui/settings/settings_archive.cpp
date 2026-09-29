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

//! Assigns the value if the storage value holds the same type.
template <typename T>
bool AssignIfHolds(const std::optional<mvvm::variant_t>& settings_value, T& value)
{
  if (!settings_value.has_value())
  {
    return false;
  }
  if (const auto stored = std::get_if<T>(&settings_value.value()); stored != nullptr)
  {
    value = *stored;
    return true;
  }
  return false;
}

}  // namespace

std::string JoinSettingsKey(const std::string& group, const std::string& name)
{
  return group.empty() ? name : group + kGroupSeparator + name;
}

mvvm::variant_t ToSettingsValue(bool value)
{
  return mvvm::variant_t{value};
}

mvvm::variant_t ToSettingsValue(mvvm::int32 value)
{
  return mvvm::variant_t{value};
}

mvvm::variant_t ToSettingsValue(double value)
{
  return mvvm::variant_t{value};
}

mvvm::variant_t ToSettingsValue(const std::string& value)
{
  return mvvm::variant_t{value};
}

mvvm::variant_t ToSettingsValue(const QByteArray& value)
{
  return mvvm::variant_t{value.toBase64().toStdString()};
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, bool& value)
{
  return AssignIfHolds(settings_value, value);
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, mvvm::int32& value)
{
  return AssignIfHolds(settings_value, value);
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, double& value)
{
  return AssignIfHolds(settings_value, value);
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, std::string& value)
{
  return AssignIfHolds(settings_value, value);
}

bool FromSettingsValue(const std::optional<mvvm::variant_t>& settings_value, QByteArray& value)
{
  std::string base64;
  if (!AssignIfHolds(settings_value, base64))
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
