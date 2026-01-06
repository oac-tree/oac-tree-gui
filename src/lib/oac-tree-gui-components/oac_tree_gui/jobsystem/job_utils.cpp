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

#include "job_utils.h"

#include <numeric>

namespace oac_tree_gui
{

std::string GetRegExpPattern(const std::vector<std::string>& data)
{
  auto fold = [](std::string result, const std::string& label)
  { return std::move(result) + '|' + label; };
  return data.empty()
             ? std::string("()")
             : "(" + std::accumulate(std::next(data.begin()), data.end(), data[0], fold) + ")";
}

}  // namespace oac_tree_gui
