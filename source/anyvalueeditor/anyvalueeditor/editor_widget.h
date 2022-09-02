/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_EDITOR_WIDGET_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_EDITOR_WIDGET_H_

#include <QWidget>
#include <memory>

class QTextEdit;
class QSplitter;

namespace mvvm
{
class ApplicationModel;
class AllItemsTreeView;
class ModelHasChangedController;
}  // namespace mvvm

namespace anyvalueeditor
{

class AnyValueEditorToolBar;
class AnyValueEditorActions;
class AnyValueItem;

class EditorWidget : public QWidget
{
  Q_OBJECT
public:
  explicit EditorWidget(QWidget* parent = nullptr);
  ~EditorWidget() override;

  void ImportAnyValueFromFile(const QString& filename);

  AnyValueItem* GetSelectedItem();

private:
  void PopulateModel();
  void UpdateJson(AnyValueItem *item);

  std::unique_ptr<mvvm::ApplicationModel> m_model;

  AnyValueEditorActions* m_actions{nullptr};
  AnyValueEditorToolBar* m_tool_bar{nullptr};
  mvvm::AllItemsTreeView* m_all_items_tree_view{nullptr};
  QTextEdit* m_text_edit{nullptr};
  QSplitter* m_splitter{nullptr};
  std::unique_ptr<mvvm::ModelHasChangedController> m_model_changed_controller;
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_EDITOR_WIDGET_H_
