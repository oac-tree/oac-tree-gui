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

#ifndef OAC_TREE_GUI_DOMAIN_DOMAIN_PLUGIN_SERVICE_H_
#define OAC_TREE_GUI_DOMAIN_DOMAIN_PLUGIN_SERVICE_H_

#include <oac_tree_gui/domain/i_domain_plugin_service.h>

namespace oac_tree_gui
{

class DomainLibraryLoader;

/**
 * @brief The DomainPluginService class is a main service to load bunch of plugins, and track their
 * content.
 *
 * TODO consider merging with DomainLibraryLoader
 *
 * @tparam LibraryLoaderT The type of the loader that knows how to load a library.
 */
template <typename LibraryLoaderT>
class DomainPluginService : public IDomainPluginService
{
public:
  /**
   * @brief Main c-tor.
   *
   * Constructs the service and initializes the library loader.
   *
   * @param library_loader The library loader to use for loading plugins.
   */
  explicit DomainPluginService(LibraryLoaderT& library_loader);
  ~DomainPluginService() override = default;

  DomainPluginService(const DomainPluginService&) = delete;
  DomainPluginService& operator=(const DomainPluginService&) = delete;
  DomainPluginService(DomainPluginService&&) = delete;
  DomainPluginService& operator=(DomainPluginService&&) = delete;

  void LoadPluginFiles(const std::vector<std::string>& plugin_file_names) override;

  std::vector<std::pair<std::string, bool>> GetPluginLoadInfo() const override;

private:
  LibraryLoaderT& m_library_loader;
};

template <typename LibraryLoaderT>
inline DomainPluginService<LibraryLoaderT>::DomainPluginService(LibraryLoaderT& library_loader)
    : m_library_loader(library_loader)
{
}

template <typename LibraryLoaderT>
inline void DomainPluginService<LibraryLoaderT>::LoadPluginFiles(
    const std::vector<std::string>& plugin_file_names)
{
  for (const auto& name : plugin_file_names)
  {
    m_library_loader.LoadLibrary(name);
  }
}

template <typename LibraryLoaderT>
inline std::vector<std::pair<std::string, bool>>
DomainPluginService<LibraryLoaderT>::GetPluginLoadInfo() const
{
  return m_library_loader.GetLibraryInfo();
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_DOMAIN_PLUGIN_SERVICE_H_
