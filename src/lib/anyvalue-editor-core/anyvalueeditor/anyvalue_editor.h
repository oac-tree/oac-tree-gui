/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef LIBANYVALUE_ANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_EDITOR_WIDGET_H_
#define LIBANYVALUE_ANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_EDITOR_WIDGET_H_

#include <QWidget>
#include <memory>

#include <anyvalueeditor/anyvalue_editor_context.h>

class QTextEdit;
class QSplitter;
class QTreeView;

namespace mvvm
{
class ApplicationModel;
class AllItemsTreeView;
class ModelHasChangedController;
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{
class AnyValueItem;
}

namespace anyvalueeditor
{

class AnyValueEditorToolBar;
class AnyValueEditorActions;

class AnyValueEditor : public QWidget
{
  Q_OBJECT
public:
  explicit AnyValueEditor(QWidget* parent = nullptr);
  ~AnyValueEditor() override;

  void ImportAnyValueFromFile(const QString& filename);

  sup::gui::AnyValueItem* GetSelectedItem() const;

private:
  void UpdateJson(sup::gui::AnyValueItem* item);

  AnyValueEditorContext CreateContext() const;

  std::unique_ptr<mvvm::ApplicationModel> m_model;

  AnyValueEditorActions* m_actions{nullptr};
  AnyValueEditorToolBar* m_tool_bar{nullptr};
  QTreeView* m_all_items_tree_view{nullptr};
  QTextEdit* m_text_edit{nullptr};
  QSplitter* m_splitter{nullptr};
  std::unique_ptr<mvvm::ModelHasChangedController> m_model_changed_controller;
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace anyvalueeditor

#endif  // LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_EDITOR_WIDGET_H_
