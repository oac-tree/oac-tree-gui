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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_SPLITTABLE_EDITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_SPLITTABLE_EDITOR_WIDGET_H_

#include <oac_tree_gui/model/component_types.h>
#include <oac_tree_gui/settings/scoped_callback.h>

#include <QWidget>
#include <memory>
#include <vector>

class QSplitter;

namespace mvvm
{
class ItemSelection;
}

namespace sup::gui
{
class IAppCommandService;
class SplittableEditorController;
class SplittableComboPanel;
}  // namespace sup::gui

namespace oac_tree_gui
{

class ISettingsManager;
class SequencerModel;
class ProcedureItem;

//! Settings group of composer panels.
constexpr auto kComposerPanelsSettingsGroup = "ProcedureSplittableEditorWidget";

/**
 * @brief The ProcedureSplittableEditorWidget holds multiple splittable ProcedureComposerTabWidgets.
 *
 * Panels are restored from application settings on construction, and from project settings on
 * project notification. Their state is provided to the settings manager on Collect of both scopes.
 * The settings manager must outlive the widget.
 */
class ProcedureSplittableEditorWidget : public QWidget
{
  Q_OBJECT

public:
  ProcedureSplittableEditorWidget(sup::gui::IAppCommandService& command_service,
                                  ISettingsManager& settings_manager,
                                  QWidget* parent_widget = nullptr);
  ~ProcedureSplittableEditorWidget() override;

  ProcedureSplittableEditorWidget(const ProcedureSplittableEditorWidget&) = delete;
  ProcedureSplittableEditorWidget& operator=(const ProcedureSplittableEditorWidget&) = delete;
  ProcedureSplittableEditorWidget(ProcedureSplittableEditorWidget&&) = delete;
  ProcedureSplittableEditorWidget& operator=(ProcedureSplittableEditorWidget&&) = delete;

  /**
   * @brief Sets the model to all panels.
   *
   * Each panel gets the procedure stored in settings, or the first procedure of the model.
   */
  void SetModel(SequencerModel* model);

  /**
   * @brief Sets the procedure to be edited.
   */
  void SetProcedure(ProcedureItem* procedure_item);

  /**
   * @brief Adds panel to the splitter.
   */
  sup::gui::SplittableComboPanel* CreatePanel();

  /**
   * @brief Insert instruction from toolbox to the instruction container.
   *
   * Will forward the call to underlying active editor.
   */
  void InsertInstructionFromToolBox(const QString& name);

  /**
   * @brief Returns the state of all procedure editor panels.
   */
  ComposerViewInfo GetComposerViewInfo() const;

  /**
   * @brief Replaces procedure editor panels with panels created from the given state.
   *
   * Procedures are assigned if the model is set.
   */
  void SetComposerViewInfo(const ComposerViewInfo& view_info);

  /**
   * @brief Returns controller of the editor, which allows to manipulate panels and their content.
   *
   * Used for testing.
   */
  sup::gui::SplittableEditorController* GetController() const;

signals:
  /**
   * @brief Notifies that the procedure selection in the focus widget has changed.
   *
   * Also notifies if focus widget changed, and the procedure is not the same as in previous focus
   * widget. This is used to propate focus procedure change to procedure list.
   */
  void focusWidgetProcedureSelectionChanged(const mvvm::ItemSelection& selection);

private:
  std::unique_ptr<sup::gui::SplittableEditorController> CreateSplitterController() const;

  //! Creates panels from application settings, or one panel if none are stored.
  void CreateInitialPanelsFromSettings();

  //! Registers providers in both scopes and the subscriber in project scope.
  void RegisterOwnSettingsInStore();

  sup::gui::IAppCommandService& m_command_service;
  ISettingsManager& m_settings_manager;
  QSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};
  std::unique_ptr<sup::gui::SplittableEditorController> m_splitter_controller;
  std::vector<ScopedCallback> m_settings_connections;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_SPLITTABLE_EDITOR_WIDGET_H_
