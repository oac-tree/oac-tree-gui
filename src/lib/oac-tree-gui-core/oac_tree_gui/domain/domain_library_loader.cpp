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

#include <oac_tree_gui/domain/domain_library_loader.h>

#include <string>
#include <utility>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace oac_tree_gui
{

namespace
{
// Return pair<success, handle>
std::pair<bool, void*> TryOpenLibrary(const std::string& path)
{
#if defined(_WIN32)
  HMODULE handle = ::LoadLibraryA(path.c_str());
  return {handle != nullptr, reinterpret_cast<void*>(handle)};
#else
  void* handle = ::dlopen(path.c_str(), RTLD_NOW);
  return {handle != nullptr, handle};
#endif
}

void CloseLibrary(void* handle)
{
  if (!handle)
  {
    return;
  }
#if defined(_WIN32)
  ::FreeLibrary(reinterpret_cast<HMODULE>(handle));
#else
  ::dlclose(handle);
#endif
}
}  // namespace

DomainLibraryLoader::DomainLibraryLoader(const std::vector<std::string>& library_names)
{
  m_loaded_libraries.reserve(library_names.size());
  m_library_handles.reserve(library_names.size());
  m_library_info.reserve(library_names.size());
  for (const auto& name : library_names)
  {
    LoadLibrary(name);
  }
}

DomainLibraryLoader::~DomainLibraryLoader()
{
  for (auto* h : m_library_handles)
  {
    CloseLibrary(h);
  }
}

std::vector<std::string> DomainLibraryLoader::GetLoadedLibraries() const
{
  return m_loaded_libraries;
}

void DomainLibraryLoader::LoadLibrary(const std::string& library_name)
{
  auto [ok, handle] = TryOpenLibrary(library_name);
  if (ok)
  {
    m_loaded_libraries.push_back(library_name);
    m_library_handles.push_back(handle);
  }
  m_library_info.push_back({library_name, ok});
}

std::vector<std::pair<std::string, bool> > DomainLibraryLoader::GetLibraryInfo() const
{
  return m_library_info;
}

}  // namespace oac_tree_gui
