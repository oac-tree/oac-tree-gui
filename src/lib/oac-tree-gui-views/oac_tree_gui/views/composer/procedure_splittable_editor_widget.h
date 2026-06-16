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

#include <sup/gui/widgets/settings_callbacks.h>

#include <QWidget>
#include <memory>

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

class SequencerModel;
class ProcedureItem;

/**
 * @brief The ProcedureSplittableEditorWidget holds multiple splittable ProcedureComposerTabWidgets.
 */
class ProcedureSplittableEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ProcedureSplittableEditorWidget(sup::gui::IAppCommandService& command_service,
                                           QWidget* parent_widget = nullptr);
  ~ProcedureSplittableEditorWidget() override;

  ProcedureSplittableEditorWidget(const ProcedureSplittableEditorWidget&) = delete;
  ProcedureSplittableEditorWidget& operator=(const ProcedureSplittableEditorWidget&) = delete;
  ProcedureSplittableEditorWidget(ProcedureSplittableEditorWidget&&) = delete;
  ProcedureSplittableEditorWidget& operator=(ProcedureSplittableEditorWidget&&) = delete;

  void SetModel(SequencerModel* model);

  /**
   * @brief Sets the procedure to be edited.
   */
  void SetProcedure(ProcedureItem* procedure_item);

  /**
   * @brief Adds panel to the splitter.
   */
  sup::gui::SplittableComboPanel *CreatePanel();

  /**
   * @brief Read settings from storage using function provided.
   */
  void ReadSettings(const sup::gui::read_variant_func_t& read_func);

  /**
   * @brief Write settings to persistent storage using function provided.
   */
  void WriteSettings(const sup::gui::write_variant_func_t& write_func);

  /**
   * @brief Insert instruction from toolbox to the instruction container.
   *
   * Will forward the call to underlying active editor.
   */
  void InsertInstructionFromToolBox(const QString& name);

  /**
   * @brief Returns composer view info.
   *
   * FIXME duplication of WriteSettings/ReadSettings
   *
   * This is the collection of UI settings for all procedure editor panels.
   */
  ComposerViewInfo GetComposerViewInfo() const;

  /**
   * @brief Sets composer view info.
   *
   * FIXME duplication of WriteSettings/ReadSettings
   *
   * This will restore the UI settings for all procedure editor panels.
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

  sup::gui::IAppCommandService& m_command_service;
  QSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};
  std::unique_ptr<sup::gui::SplittableEditorController> m_splitter_controller;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_SPLITTABLE_EDITOR_WIDGET_H_
