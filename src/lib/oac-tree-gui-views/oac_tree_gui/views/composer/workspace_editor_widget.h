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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_

#include "oac_tree_gui/components/component_types.h"
#include "oac_tree_gui/model/procedure_item.h"

#include <QWidget>

class QTreeView;
class QLineEdit;

namespace mvvm
{
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{
class CustomHeaderView;
class VisibilityAgentBase;
class IAppCommandService;
}  // namespace sup::gui

namespace oac_tree_gui
{

class WorkspaceItem;
class WorkspaceEditor;
class WorkspaceViewComponentProvider;

/**
 * @brief The WorkspaceEditorWidget class is intended for editing of workspace variables.
 *
 * It is located at the right of SequencerComposerView (second main view). Defines set of actions to
 * add and remove variables.
 */
class WorkspaceEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WorkspaceEditorWidget(sup::gui::IAppCommandService& command_service,
                                 WorkspacePresentationType presentation,
                                 QWidget* parent_widget = nullptr);
  ~WorkspaceEditorWidget() override;

  void SetWorkspaceItem(WorkspaceItem* workspace_item);

  void SetWorkspaceEditType(WorkspaceEditType edit_type);

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  void SetupConnections();
  void SetupTree();
  void AdjustTreeAppearance();

  /**
   * @brief Creates tree component provider for given type of workspace presentation.
   *
   * Tree will be equipped with the model, generating either variable tree, or variable table.
   */
  std::unique_ptr<WorkspaceViewComponentProvider> CreateProvider(
      WorkspacePresentationType presentation) const;

  /**
   * @brief Summons custom tree context menu to modify attributes.
   */
  void OnTreeContextMenuRequest(const QPoint& point);

  void SetWorkspaceItemIntern(WorkspaceItem* workspace_item);

  QTreeView* m_tree_view{nullptr};
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<WorkspaceViewComponentProvider> m_component_provider;
  sup::gui::VisibilityAgentBase* m_visibility_agent{nullptr};
  QLineEdit* m_line_edit{nullptr};
  WorkspaceEditor* m_editor{nullptr};

  WorkspaceItem* m_workspace_item{nullptr};
  WorkspaceEditType m_edit_type;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
