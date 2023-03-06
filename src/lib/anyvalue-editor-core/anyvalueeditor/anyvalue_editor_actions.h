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

#ifndef LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
#define LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_

#include <anyvalueeditor/anyvalue_editor_context.h>

#include <mvvm/model/application_model.h>

#include <QObject>

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{
class AnyValueItem;
}

namespace anyvalueeditor
{

//! The AnyValueEditorActions class implements actions for AnyValueEditor that can be triggered from
//! its main toolbar.
//!
//! It allows to add to the model AnyValueItems representing scalars, structs, and arrays. Depending
//! on passed parameters, items can be added either as top-level items, or as a field to already
//! existing items. The class rely on callbacks to query currently selected item and to report an
//! error if the action is not possible.

class AnyValueEditorActions : public QObject
{
  Q_OBJECT

public:
  AnyValueEditorActions(AnyValueEditorContext context, mvvm::ApplicationModel* model,
                        QObject* parent);

  void OnAddAnyValueStruct(bool selected_as_parent);

  void OnAddAnyValueArray(bool selected_as_parent);

  void OnAddAnyValueScalar(const std::string& scalar_type, bool selected_as_parent);

  void OnRemoveSelected();

private:
  void AddAnyValueStruct(mvvm::SessionItem* parent);

  template <typename T>
  T* AddAnyValueItem(mvvm::SessionItem* parent);

  mvvm::ApplicationModel* m_model{nullptr};
  AnyValueEditorContext m_context;
};

template <typename T>
inline T* AnyValueEditorActions::AddAnyValueItem(mvvm::SessionItem* parent)
{
  T* result{nullptr};

  if (!parent)
  {
    return nullptr;
  }

  try
  {
    result = m_model->InsertItem<T>(parent, mvvm::TagIndex::Append());
  }
  catch (const std::exception& ex)
  {
    auto message = sup::gui::CreateInvalidOperationMessage("Can't insert item");
    m_context.send_message_callback(message);
  }

  return result;
}

}  // namespace anyvalueeditor

#endif  // LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
