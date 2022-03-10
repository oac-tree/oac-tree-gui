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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITORACTIONS_H
#define ANYVALUEEDITOR_ANYVALUEEDITORACTIONS_H

#include <QObject>

class QItemSelectionModel;

namespace mvvm
{
class ApplicationModel;
class ViewModel;
}

namespace anyvalueeditor
{

//! Actions for AnyValueEditor

class AnyValueEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit AnyValueEditorActions(mvvm::ApplicationModel* model, QObject* parent = nullptr);

  void OnAddAnyValue();

  void OnAddField();

  void OnInsertField();

  void OnRemoveSelected();

  void SetViewModel(mvvm::ViewModel* view_model);

  void SetSelectionModel(QItemSelectionModel* selection_model);

private:
  mvvm::ApplicationModel* m_model{nullptr};
  mvvm::ViewModel* m_view_model{nullptr};
  QItemSelectionModel* m_selection_model{nullptr};
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITORACTIONS_H
