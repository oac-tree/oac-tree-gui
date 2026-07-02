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

std::unique_ptr<QStandardItem> CreateItem(const QString& text)
{
  auto result = std::make_unique<QStandardItem>(text);
  result->setEditable(false);
  result->setToolTip(text);
  return result;
}

}  // namespace

JobInfoViewModel::JobInfoViewModel(QObject* parent_object) : QStandardItemModel(parent_object) {}

void JobInfoViewModel::PopulateModel(const std::vector<std::string>& job_list)
{
  clear();

  auto parent_item = invisibleRootItem();
  for (std::size_t index = 0; index < job_list.size(); ++index)
  {
    auto index_item = CreateItem(QString::number(index));
    auto name_item = CreateItem(QString::fromStdString(job_list[index]));
    parent_item->appendRow({index_item.release(), name_item.release()});
  }
}

}  // namespace oac_tree_gui
