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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_

#include <QObject>

namespace mvvm
{
class ApplicationModel;
}

namespace anyvalueeditor
{

class AnyValueItem;

//! Actions for AnyValueEditor

class AnyValueEditorActions : public QObject
{
  Q_OBJECT

public:
  explicit AnyValueEditorActions(mvvm::ApplicationModel* model, QObject* parent = nullptr);

  void OnAddAnyValueStruct();
  void OnAddAnyValueArray();
  void OnAddAnyValueScalar(const std::string& scalar_type);

  void OnAddField();

  void OnInsertField();

  void OnRemoveSelected();

  void SetSelectedItem(AnyValueItem* item);

private:
  mvvm::ApplicationModel* m_model{nullptr};
  AnyValueItem* m_selected_item{nullptr};
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
