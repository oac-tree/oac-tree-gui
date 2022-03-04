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

#ifndef ANYVALUEEDITOR_EDITORWIDGET_H
#define ANYVALUEEDITOR_EDITORWIDGET_H

#include <QWidget>

#include <memory>

class QTreeView;

namespace mvvm
{
class ApplicationModel;
class ViewModel;
class ViewModelDelegate;
}

namespace anyvalueeditor
{

class AnyValueEditorToolBar;

class EditorWidget : public QWidget
{
  Q_OBJECT
public:
  explicit EditorWidget(QWidget* parent = nullptr);
  ~EditorWidget() override;

  void ImportAnyValueFromFile(const QString& filename);

private:
  void PopulateModel();

  AnyValueEditorToolBar* m_tool_bar{nullptr};
  QTreeView* m_tree_view{nullptr};

  std::unique_ptr<mvvm::ApplicationModel> m_model;
  std::unique_ptr<mvvm::ViewModel> m_view_model;
  std::unique_ptr<mvvm::ViewModelDelegate> m_delegate;
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_EDITORWIDGET_H
