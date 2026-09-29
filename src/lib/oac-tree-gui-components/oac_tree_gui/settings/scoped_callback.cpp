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

#include "scoped_callback.h"

#include <utility>

namespace oac_tree_gui
{

ScopedCallback::ScopedCallback(std::function<void()> callback) : m_callback(std::move(callback)) {}

ScopedCallback::~ScopedCallback()
{
  Disconnect();
}

ScopedCallback::ScopedCallback(ScopedCallback&& other) noexcept
    : m_callback(std::exchange(other.m_callback, nullptr))
{
}

ScopedCallback& ScopedCallback::operator=(ScopedCallback&& other) noexcept
{
  if (this != &other)
  {
    Disconnect();
    m_callback = std::exchange(other.m_callback, nullptr);
  }
  return *this;
}

bool ScopedCallback::IsConnected() const
{
  return static_cast<bool>(m_callback);
}

void ScopedCallback::Disconnect()
{
  if (auto func = std::exchange(m_callback, nullptr); func)
  {
    func();
  }
}

}  // namespace oac_tree_gui
