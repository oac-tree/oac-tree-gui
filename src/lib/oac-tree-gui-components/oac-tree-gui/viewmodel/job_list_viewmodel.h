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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWMODEL_JOB_LIST_VIEWMODEL_H_
#define OAC_TREE_GUI_VIEWMODEL_JOB_LIST_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{
class ISessionModel;
}

namespace oac_tree_gui
{

/**
 * @brief The JobListViewModel class is a view model to show JobItem with two columns: displayName
 * and status.
 */
class MVVM_VIEWMODEL_EXPORT JobListViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit JobListViewModel(mvvm::ISessionModel* model, QObject* parent_object = nullptr);
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_JOB_LIST_VIEWMODEL_H_
