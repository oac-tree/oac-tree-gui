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

#ifndef LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ADD_FIELD_DIALOG_H_
#define LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ADD_FIELD_DIALOG_H_

#include <QDialog>

class QLineEdit;
class QComboBox;

namespace sup::gui
{
struct FieldContext;
}

namespace anyvalueeditor
{
class AddFieldDialog : public QDialog
{
  Q_OBJECT
public:
  explicit AddFieldDialog(QWidget* parent = nullptr);

  sup::gui::FieldContext GetFieldContext() const;

private:
  QLineEdit* m_name_field;
  QComboBox* m_type_combo;
  QComboBox* m_scalar_type_combo;
};

}  // namespace anyvalueeditor

#endif  // LIBANYVALUE_EDITOR_CORE_ANYVALUEEDITOR_ADD_FIELD_DIALOG_H_
