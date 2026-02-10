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

#ifndef OAC_TREE_GUI_VIEWMODEL_PROCEDURE_LIST_VIEWMODEL_H_
#define OAC_TREE_GUI_VIEWMODEL_PROCEDURE_LIST_VIEWMODEL_H_

#include <oac_tree_gui/viewmodel/flatlist_viewmodel.h>

namespace oac_tree_gui
{

/**
 * @brief The ProcedureListViewModel class is a view model to show the list of procedures.
 */
class ProcedureListViewModel : public FlatListViewModel
{
  Q_OBJECT

public:
  explicit ProcedureListViewModel(mvvm::ISessionModel* model, QObject* parent_object = nullptr);
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_PROCEDURE_LIST_VIEWMODEL_H_
