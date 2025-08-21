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

#ifndef OAC_TREE_GUI_DOMAIN_DOMAIN_LIBRARY_LOADER_H_
#define OAC_TREE_GUI_DOMAIN_DOMAIN_LIBRARY_LOADER_H_

#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The DomainLibraryLoader class is responsible for handling shared library loading.
 *
 * It tracks successfully loaded libraries, it unloads libraries on own destruction.
 */
class DomainLibraryLoader
{
public:
  /**
   * @brief Default c-tor.
   */
  DomainLibraryLoader() = default;

  /**
   * @brief Constructs the object and loads libraries.
   *
   * Loads the list of shared libraries on own construction.
   *
   * @param library_names List of all libraries to load.
   */
  explicit DomainLibraryLoader(const std::vector<std::string>& library_names);
  ~DomainLibraryLoader();

  DomainLibraryLoader(const DomainLibraryLoader&) = delete;
  DomainLibraryLoader& operator=(const DomainLibraryLoader&) = delete;
  DomainLibraryLoader(DomainLibraryLoader&&) noexcept = default;
  DomainLibraryLoader& operator=(DomainLibraryLoader&&) noexcept = default;

  /**
   * @brief Returns names of successfully loaded libraries.
   */
  std::vector<std::string> GetLoadedLibraries() const;

  /**
   * @brief Loads the library with the given name.
   */
  void LoadLibrary(const std::string& library_name);

  /**
   * @brief Returns the list of all libraries we've tried to load, and their success status.
   */
  std::vector<std::pair<std::string, bool>> GetLibraryInfo() const;

private:
  std::vector<std::string> m_loaded_libraries;
  std::vector<std::pair<std::string, bool>> m_library_info;
  std::vector<void*> m_library_handles;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_DOMAIN_LIBRARY_LOADER_H_
