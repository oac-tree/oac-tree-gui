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

#ifndef OAC_TREE_GUI_VIEWMODEL_JOB_INFO_VIEWMODEL_H_
#define OAC_TREE_GUI_VIEWMODEL_JOB_INFO_VIEWMODEL_H_

#include <QStandardItemModel>
#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The JobInfoViewModel class provides a viewmodel with information about remote jobs.
 */
class JobInfoViewModel : public QStandardItemModel
{
  Q_OBJECT

public:
  explicit JobInfoViewModel(QObject* parent_object = nullptr);

  /**
   * @brief Populate model so it contains a list of job names.
   *
   * This will clear the model from the previous content.
   */
  void PopulateModel(const std::vector<std::string>& job_list);
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_JOB_INFO_VIEWMODEL_H_
