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

#include <mvvm/model/application_model.h>

#include <sup/gui/anyvalueeditor/anyvalue_editor_context.h>

#include <QObject>

namespace mvvm
{
class SessionItem;
}

namespace sup::gui
{

class AnyValueItem;

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

  void OnAddAnyValueStruct();

  void OnAddAnyValueArray();

  void OnAddAnyValueScalar(const std::string& scalar_type);

  void OnRemoveSelected();

  void OnImportFromFileRequest(const std::string& file_name);

  void OnExportToFileRequest(const std::string& file_name);

  void SetInitialValue(const AnyValueItem& item);

  AnyValueItem* GetTopItem();

  sup::gui::AnyValueItem* GetSelectedItem() const;

private:
  mvvm::SessionItem* GetParent() const;
  void SendMessage(const std::string& text, const std::string& informative = {},
                   const std::string& details = {});

  template <typename T>
  T* AddAnyValueItem(const std::string& item_name);

  mvvm::ApplicationModel* m_model{nullptr};
  AnyValueEditorContext m_context;
};

template <typename T>
inline T* AnyValueEditorActions::AddAnyValueItem(const std::string& item_name)
{
  T* result{nullptr};

  if (!GetSelectedItem() && GetTopItem())
  {
    SendMessage("Please select an item where you want to add a field");
    return nullptr;
  }

  if (auto parent = GetParent(); parent)
  {
    try
    {
      result = m_model->InsertItem<T>(parent, mvvm::TagIndex::Append());
      result->SetDisplayName(item_name);
    }
    catch (const std::exception& ex)
    {
      SendMessage("Can't add item `" + item_name + "' to current selection", "", ex.what());
    }
  }

  return result;
}

}  // namespace sup::gui

#endif  // LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
