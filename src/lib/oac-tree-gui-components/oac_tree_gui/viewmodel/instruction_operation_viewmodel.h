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

#ifndef OAC_TREE_GUI_VIEWMODEL_INSTRUCTION_OPERATION_VIEWMODEL_H_
#define OAC_TREE_GUI_VIEWMODEL_INSTRUCTION_OPERATION_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{
class ISessionModel;
}

namespace oac_tree_gui
{

//! View model to show instruction tree with three columns: display_name, name and status.

class MVVM_VIEWMODEL_EXPORT InstructionOperationViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit InstructionOperationViewModel(mvvm::ISessionModel* model,
                                         QObject* parent_object = nullptr);

  /**
   * @brief Returns index of a column used to render breakpoints.
   */
  static int GetBreakpointColumn();
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWMODEL_INSTRUCTION_OPERATION_VIEWMODEL_H_
