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

#include "cmake_info.h"

#include "cmake_info_constants.h"  // from <build>/autogen

namespace oac_tree_gui::test
{

std::string CMakeSourceDir()
{
  return kCMakeSourceDir;
}

std::string CMakeBinaryDir()
{
  return kCMakeBinaryDir;
}

std::string TestOutputDir()
{
  return kTestOutputDir;
}

std::string ProjectSourceDir()
{
  return kProjectSourceDir;
}

std::string ProjectResourceDir()
{
  return ProjectSourceDir() + "/test/resources/functional";
}

}  // namespace oac_tree_gui::test
