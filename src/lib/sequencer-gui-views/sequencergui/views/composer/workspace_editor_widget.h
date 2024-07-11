/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_

#include "sequencergui/model/procedure_item.h"
#include <QWidget>
#include <memory>

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
}  // namespace sup::gui

namespace sequencergui
{

class WorkspaceItem;
class VariableItem;
class WorkspaceEditorContext;
class WorkspaceEditorActionHandler;
class WorkspaceEditorActions;
class AttributeEditorContext;
class AttributeEditorActions;
class AttributeEditorActionHandler;
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
  explicit WorkspaceEditorWidget(QWidget* parent = nullptr);
  ~WorkspaceEditorWidget() override;

  void SetWorkspaceItem(WorkspaceItem* workspace_item);

  mvvm::SessionItem* GetSelectedItem() const;

private:
  void AdjustTreeAppearance();

  /**
   * @brief Summons custom tree context menu to modify attributes.
   */
  void OnTreeContextMenuRequest(const QPoint& point);

  void SetWorkspaceItemIntern(WorkspaceItem* workspace_item);

  void SetupConnections();
  WorkspaceEditorContext CreateWorkspaceEditorContext();
  AttributeEditorContext CreateAttributeEditorContext();

  QTreeView* m_tree_view{nullptr};
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<WorkspaceViewComponentProvider> m_component_provider;
  WorkspaceItem* m_workspace_item{nullptr};

  WorkspaceEditorActionHandler* m_action_handler{nullptr};
  WorkspaceEditorActions* m_editor_actions{nullptr};

  AttributeEditorActionHandler* m_attribute_action_handler{nullptr};
  AttributeEditorActions* m_attribute_actions{nullptr};

  sup::gui::VisibilityAgentBase* m_visibility_agent{nullptr};
  QLineEdit* m_line_edit{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
