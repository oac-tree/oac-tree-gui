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

#ifndef LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
#define LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_

#include <QObject>
#include <functional>

namespace mvvm
{
class ApplicationModel;
}

namespace sup::gui
{
class AnyValueItem;
}

namespace anyvalueeditor
{

//! Actions for AnyValueEditor

class AnyValueEditorActions : public QObject
{
  Q_OBJECT

public:
  using callback_t = std::function<sup::gui::AnyValueItem*()>;
  AnyValueEditorActions(mvvm::ApplicationModel* model, QObject* parent,
                                 callback_t get_selected_callback);

  void OnAddAnyValueStruct(bool to_selected);

  void OnAddAnyValueArray(bool to_selected);

  void OnAddAnyValueScalar(const std::string& scalar_type, bool to_selected);

  void OnRemoveSelected();

private:
  mvvm::ApplicationModel* m_model{nullptr};
  callback_t m_get_selected_callback;
};

}  // namespace anyvalueeditor

#endif  // LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
