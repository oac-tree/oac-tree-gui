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

#ifndef OAC_TREE_GUI_VIEWS_PROCEDURE_SPLITTABLE_EDITOR_WIDGET_H_
#define OAC_TREE_GUI_VIEWS_PROCEDURE_SPLITTABLE_EDITOR_WIDGET_H_

#include <sup/gui/widgets/settings_callbacks.h>

#include <QWidget>
#include <memory>

class QSplitter;

namespace sup::gui
{
class IAppCommandService;
}

namespace oac_tree_gui
{

class SequencerModel;
class ProcedureComposerComboPanel;
class ProcedureItem;

template <typename WidgetT>
class WidgetFocusHandler;

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

  void SetModel(SequencerModel* model);

  /**
   * @brief Sets the procedure to be edited.
   */
  void SetProcedure(ProcedureItem* procedure_item);

  /**
   * @brief Gets the internal splitter.
   */
  QSplitter* GetSplitter() const;

  /**
   * @brief Adds a new widget to the splitter.
   *
   * @param after_widget If nullptr, the new widget is added at the end. Otherwise, it is added
   * after the specified widget.
   */
  void CreatePanel(ProcedureComposerComboPanel* after_widget = nullptr);

  /**
   * @brief Closes and deletes the specified widget in the splitter.
   */
  void ClosePanel(ProcedureComposerComboPanel* widget_to_close);

  /**
   * @brief Returns the widget currently in focus.
   */
  ProcedureComposerComboPanel* GetFocusWidget();

  /**
   * @brief Sets the widget in focus.
   */
  void SetFocusWidget(ProcedureComposerComboPanel* widget);

  /**
   * @brief Read settings from storage using function provided.
   */
  void ReadSettings(const sup::gui::read_variant_func_t& read_func);

  /**
   * @brief Write settings to persistent storage using function provided.
   */
  void WriteSettings(const sup::gui::write_variant_func_t& write_func);

signals:
  void focusWidgetProcedureSelectionChanged(oac_tree_gui::ProcedureItem* item);

private:
  /**
   * @brief Creates a new procedure editor widget.
   */
  std::unique_ptr<ProcedureComposerComboPanel> CreateProcedureEditor();

  /**
   * @brief Notifies that the procedure selection in the focus widget has changed.
   *
   * Also notifies if focus widget changed, and the procedure is not the same as in previous focus
   * widget. This is used to propate focus procedure change to procedure list.
   */
  void NotifyFocusWidgetProcedureSelectionChanged(ProcedureItem* item);

  sup::gui::IAppCommandService& m_command_service;
  QSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};
  std::unique_ptr<WidgetFocusHandler<ProcedureComposerComboPanel>> m_focus_handler;
  bool m_block_selection_change_notification{false};

  //! cached value of current procedure item in focus widget
  ProcedureItem* m_procedure_item_in_focus_cache{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_PROCEDURE_SPLITTABLE_EDITOR_WIDGET_H_
