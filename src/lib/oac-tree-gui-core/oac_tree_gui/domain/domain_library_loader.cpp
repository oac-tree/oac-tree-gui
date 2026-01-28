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

#include <oac_tree_gui/domain/domain_library_loader.h>

#include <sup/oac-tree/exceptions.h>
#include <sup/oac-tree/generic_utils.h>
#include <sup/oac-tree/sequence_parser.h>

namespace oac_tree_gui
{

namespace
{

// Return pair<success, handle>
std::pair<bool, DomainLibraryLoader::LibraryHandle> TryOpenLibrary(const std::string& path)
{
  std::pair<bool, DomainLibraryLoader::LibraryHandle> result{true, nullptr};

  try
  {
    // will automatically track loaded instructions and variables
    result.second = sup::oac_tree::LoadPlugin(path);
    result.first = true;
  }
  catch (const sup::oac_tree::InvalidOperationException&)
  {
    // parsing problems
    result.first = false;
  }
  catch (const sup::oac_tree::RuntimeException&)
  {
    // dlopen returned nullptr
    result.first = false;
  }

  return result;
}

void CloseLibrary(DomainLibraryLoader::LibraryHandle handle)
{
  if (handle != nullptr)
  {
    sup::oac_tree::utils::UnloadLibrary(handle);
  }
}

}  // namespace

DomainLibraryLoader::DomainLibraryLoader(const std::vector<std::string>& library_names)
{
  m_library_handles.reserve(library_names.size());
  m_library_info.reserve(library_names.size());
  for (const auto& name : library_names)
  {
    LoadLibrary(name);
  }
}

DomainLibraryLoader::~DomainLibraryLoader() = default;

void DomainLibraryLoader::LoadPluginFiles(const std::vector<std::string>& plugin_file_names)
{
  for (const auto& name : plugin_file_names)
  {
    LoadLibrary(name);
  }
}

void DomainLibraryLoader::LoadLibrary(const std::string& library_name)
{
  auto [ok, handle] = TryOpenLibrary(library_name);

  if (ok)
  {
    m_library_handles.push_back(handle);
  }
  m_library_info.emplace_back(library_name, ok);
}

std::vector<std::pair<std::string, bool> > DomainLibraryLoader::GetPluginLoadInfo() const
{
  return m_library_info;
}

void DomainLibraryLoader::UnloadAll()
{
  for (auto h : m_library_handles)
  {
    CloseLibrary(h);
  }
  m_library_handles.clear();
  m_library_info.clear();
}

}  // namespace oac_tree_gui
