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

#ifndef SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
#define SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_

#include <QWidget>
#include <memory>

class QTreeView;

namespace mvvm
{
class ItemViewComponentProvider;
class SessionItem;
}  // namespace mvvm

namespace sup::gui
{
class CustomHeaderView;
class VisibilityAgentBase;
}  // namespace sup::gui

namespace sequencergui
{

class ProcedureItem;
class VariableItem;
class WorkspaceEditorContext;
class WorkspaceEditorActionHandler;
class WorkspaceEditorActions;

//! List of workspace variables at the right of SequencerComposerView.
//! Defines set of actions to add/remove variables. Part of ComposerProcedureEditor.

class WorkspaceEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit WorkspaceEditorWidget(QWidget* parent = nullptr);
  ~WorkspaceEditorWidget() override;

  void SetProcedure(ProcedureItem* procedure);

  mvvm::SessionItem* GetSelectedItem() const;

private:
  void ReadSettings();
  void WriteSettings();
  void AdjustColumnWidth();
  void SetProcedureIntern(ProcedureItem* procedure);

  void SetupConnections();
  WorkspaceEditorContext CreateWorkspaceEditorContext();

  QTreeView* m_tree_view{nullptr};
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
  ProcedureItem* m_procedure{nullptr};

  WorkspaceEditorActions* m_editor_actions{nullptr};
  std::unique_ptr<WorkspaceEditorActionHandler> m_action_handler;

  sup::gui::VisibilityAgentBase* m_visibility_agent{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_WORKSPACE_EDITOR_WIDGET_H_
