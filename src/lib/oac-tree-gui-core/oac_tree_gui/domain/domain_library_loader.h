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

//! @file
//! Placeholder for a class responsible for handling domain library loading.

#include <string>
#include <vector>

namespace oac_tree_gui
{

class DomainLibraryLoader
{
public:
  // Loads all libraries passed in; tracks successfully loaded names.
  explicit DomainLibraryLoader(const std::vector<std::string>& library_names);
  ~DomainLibraryLoader();

  DomainLibraryLoader(const DomainLibraryLoader&) = delete;
  DomainLibraryLoader& operator=(const DomainLibraryLoader&) = delete;
  DomainLibraryLoader(DomainLibraryLoader&&) noexcept = default;
  DomainLibraryLoader& operator=(DomainLibraryLoader&&) noexcept = default;

  // Returns names of successfully loaded libraries (as provided to the ctor).
  const std::vector<std::string>& GetLoadedLibraries() const { return m_loaded_libraries; }

private:
  std::vector<std::string> m_loaded_libraries;
  std::vector<void*> m_handles;  // platform-specific library handles kept until destruction
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_DOMAIN_LIBRARY_LOADER_H_
