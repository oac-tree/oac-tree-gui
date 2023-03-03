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

#include <QObject>

namespace mvvm
{
class ApplicationModel;
class SessionItem;
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
  AnyValueEditorActions(AnyValueEditorContext context, mvvm::ApplicationModel* model,
                        QObject* parent);

//  void OnCreateAnyValueStruct();
//  void OnAddStructField();

  void OnAddAnyValueStruct(bool selected_as_parent);

  void OnAddAnyValueArray(bool to_selected);

  void OnAddAnyValueScalar(const std::string& scalar_type, bool to_selected);

  void OnRemoveSelected();

private:
  void AddAnyValueStruct(mvvm::SessionItem* parent);

  mvvm::ApplicationModel* m_model{nullptr};
  AnyValueEditorContext m_context;
};

}  // namespace anyvalueeditor

#endif  // LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ANYVALUE_EDITOR_ACTIONS_H_
