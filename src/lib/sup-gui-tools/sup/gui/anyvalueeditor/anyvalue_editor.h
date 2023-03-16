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

#include <sup/gui/anyvalueeditor/anyvalue_editor_context.h>

#include <QWidget>
#include <memory>

class QSplitter;
class QTreeView;

namespace mvvm
{
class ApplicationModel;
class AllItemsTreeView;
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{

class AnyValueItem;
class AnyValueEditorToolBar;
class AnyValueEditorActions;
class AnyValueEditorTextPanel;

class AnyValueEditor : public QWidget
{
  Q_OBJECT

public:
  explicit AnyValueEditor(QWidget* parent = nullptr);
  ~AnyValueEditor() override;

  void OnImportFromFileRequest();

  sup::gui::AnyValueItem* GetSelectedItem() const;

  void SetInitialValue(const sup::gui::AnyValueItem& item);

  AnyValueItem* GetTopItem();

private:
  void SetupConnections();
  void ImportAnyValueFromFile(const QString& filename);

  AnyValueEditorContext CreateActionContext() const;

  std::unique_ptr<mvvm::ApplicationModel> m_model;

  AnyValueEditorActions* m_actions{nullptr};
  AnyValueEditorToolBar* m_tool_bar{nullptr};
  QTreeView* m_tree_view{nullptr};
  AnyValueEditorTextPanel* m_text_edit{nullptr};
  QSplitter* m_splitter{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace sup::gui

#endif  // LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_EDITOR_WIDGET_H_
