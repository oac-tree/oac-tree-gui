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

#ifndef SEQUENCERGUI_VIEWMODEL_WORKSPACE_EDITOR_VIEWMODEL_H_
#define SEQUENCERGUI_VIEWMODEL_WORKSPACE_EDITOR_VIEWMODEL_H_

#include <mvvm/viewmodel/viewmodel.h>

namespace mvvm
{
class ISessionModel;
}

namespace oac_tree_gui
{

/**
 * @brief The WorkspaceEditorViewModel class forms editable three-columns tree with Workspace
 * variables.
 *
 * The variable is represented by type and editable name, with variable attributes in a branch
 * below.
 */
class MVVM_VIEWMODEL_EXPORT WorkspaceEditorViewModel : public mvvm::ViewModel
{
  Q_OBJECT

public:
  explicit WorkspaceEditorViewModel(mvvm::ISessionModel* model, bool show_hidden = false,
                                    QObject* parent_object = nullptr);

  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_VIEWMODEL_WORKSPACE_EDITOR_VIEWMODEL_H_
