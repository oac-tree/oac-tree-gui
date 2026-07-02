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

#include "job_info_viewmodel.h"

#include <memory>

namespace oac_tree_gui
{

namespace
{

std::unique_ptr<QStandardItem> CreateItem(const std::string& name)
{
  auto result = std::make_unique<QStandardItem>(QString::fromStdString(name));
  result->setEditable(false);
  result->setToolTip(QString::fromStdString(name));
  return result;
}

}  // namespace

JobInfoViewModel::JobInfoViewModel(QObject* parent_object) : QStandardItemModel(parent_object) {}

void JobInfoViewModel::PopulateModel(const std::vector<std::string>& job_list)
{
  clear();

  auto parent_item = invisibleRootItem();
  for (const auto& name : job_list)
  {
    parent_item->appendRow(CreateItem(name).release());
  }
}

}  // namespace oac_tree_gui
