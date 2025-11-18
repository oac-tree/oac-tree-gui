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

#include <QWidget>
#include <memory>

class QSplitter;

namespace oac_tree_gui
{

class SequencerModel;
class ProcedureComposerComboPanel;

template <typename WidgetT>
class WidgetFocusHandler;

/**
 * @brief The ProcedureSplittableEditorWidget holds multiple splittable ProcedureComposerTabWidgets.
 */
class ProcedureSplittableEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit ProcedureSplittableEditorWidget(QWidget* parent_widget = nullptr);
  ~ProcedureSplittableEditorWidget() override;

  void SetModel(SequencerModel* model);

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

private:
  /**
   * @brief Creates a new procedure editor widget.
   */
  std::unique_ptr<QWidget> CreateProcedureEditor();

  QSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};
  std::unique_ptr<WidgetFocusHandler<ProcedureComposerComboPanel>> m_focus_handler;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_PROCEDURE_SPLITTABLE_EDITOR_WIDGET_H_
