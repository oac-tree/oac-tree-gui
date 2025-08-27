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

#ifndef OAC_TREE_GUI_DOMAIN_PLUGIN_SERVICE_H_
#define OAC_TREE_GUI_DOMAIN_PLUGIN_SERVICE_H_

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/domain/i_domain_plugin_service.h>

namespace oac_tree_gui
{

class DomainLibraryLoader;
class DomainObjectTypeRegistry;

/**
 * @brief The DomainPluginService class is a main service to load bunch of plugins, and track their
 * content.
 *
 * Fully constructed
 *
 * @tparam LibraryLoaderT The type of the loader that knows how to load a library.
 * @taparm ObjectRegistryT The type of the object registry.
 */
template <typename LibraryLoaderT, typename ObjectRegistryT>
class DomainPluginService : public IDomainPluginService
{
public:
  /**
   * @brief Main c-tor.
   *
   * Constructs the service and initializes the library loader and object registry.
   *
   * @param library_loader The library loader to use for loading plugins.
   * @param object_regisrty The object registry to use for tracking plugin content.
   */
  DomainPluginService(LibraryLoaderT& library_loader, ObjectRegistryT& object_registry);
  ~DomainPluginService() override = default;

  void LoadPluginFiles(const std::vector<std::string>& plugin_file_names) override;

  std::vector<std::string> GetLoadedPlugins() const override;

  std::vector<std::pair<std::string, bool>> GetPluginLoadInfo() const override;

  std::vector<std::string> GetObjectNames(const std::string& plugin_name) const override;

  std::optional<std::string> GetPluginName(const std::string& object_name) const override;

private:
  /**
   * @brief Asks object type registry to update its content with the given plugin name.
   *
   * The method shall be called each time after new plugin is loaded.
   */
  void UpdateObjectTypeRegistry(const std::string& plugin_name);

  LibraryLoaderT& m_library_loader;
  ObjectRegistryT& m_object_type_registry;
};

template <typename LibraryLoaderT, typename ObjectRegistryT>
inline DomainPluginService<LibraryLoaderT, ObjectRegistryT>::DomainPluginService(
    LibraryLoaderT& library_loader, ObjectRegistryT& object_registry)
    : m_library_loader(library_loader), m_object_type_registry(object_registry)
{
}

template <typename LibraryLoaderT, typename ObjectRegistryT>
inline void DomainPluginService<LibraryLoaderT, ObjectRegistryT>::LoadPluginFiles(
    const std::vector<std::string>& plugin_file_names)
{
  // populates the registry with initial list of instructions and variables available via oac-tree
  // core library.
  UpdateObjectTypeRegistry(domainconstants::kCorePluginName);
  for (const auto& name : plugin_file_names)
  {
    m_library_loader.LoadLibrary(name);
    UpdateObjectTypeRegistry(GetPluginNameFromFileName(name));
  }
}

template <typename LibraryLoaderT, typename ObjectRegistryT>
inline std::vector<std::string>
DomainPluginService<LibraryLoaderT, ObjectRegistryT>::GetLoadedPlugins() const
{
  return m_library_loader.GetLoadedLibraries();
}

template <typename LibraryLoaderT, typename ObjectRegistryT>
inline std::vector<std::pair<std::string, bool>>
DomainPluginService<LibraryLoaderT, ObjectRegistryT>::GetPluginLoadInfo() const
{
  return m_library_loader.GetLibraryInfo();
}

template <typename LibraryLoaderT, typename ObjectRegistryT>
inline std::vector<std::string>
DomainPluginService<LibraryLoaderT, ObjectRegistryT>::GetObjectNames(
    const std::string& plugin_name) const
{
  return m_object_type_registry.GetObjectNames(plugin_name);
}

template <typename LibraryLoaderT, typename ObjectRegistryT>
inline std::optional<std::string>
DomainPluginService<LibraryLoaderT, ObjectRegistryT>::GetPluginName(
    const std::string& object_name) const
{
  return m_object_type_registry.GetPluginName(object_name);
}

template <typename LibraryLoaderT, typename ObjectRegistryT>
inline void DomainPluginService<LibraryLoaderT, ObjectRegistryT>::UpdateObjectTypeRegistry(
    const std::string& plugin_name)
{
  m_object_type_registry.Update(plugin_name);
}

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_PLUGIN_SERVICE_H_
