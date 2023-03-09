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

#ifndef SUP_PVMONITOR_CORE_ANYVALUEEEDITOR_DIALOG_H_
#define SUP_PVMONITOR_CORE_ANYVALUEEEDITOR_DIALOG_H_

#include <QDialog>

class QLineEdit;
class QComboBox;
class QBoxLayout;

namespace sup::gui
{
class AnyValueEditor;
}

namespace suppvmonitor
{

class AnyValueEditorDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AnyValueEditorDialog(QWidget* parent = nullptr);

private:
  QBoxLayout* CreateButtonLayout();

  sup::gui::AnyValueEditor* m_anyvalue_editor{nullptr};
};

}  // namespace suppvmonitor

#endif  // SUP_PVMONITOR_CORE_ANYVALUEEEDITOR_DIALOG_H_
