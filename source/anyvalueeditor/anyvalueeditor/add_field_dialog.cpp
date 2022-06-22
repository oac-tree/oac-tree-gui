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

#include "anyvalueeditor/add_field_dialog.h"

#include "anyvalueeditor/anyvalue_utils.h"

#include <mvvm/widgets/widget_utils.h>

#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

namespace
{
QStringList CreateListOfScalars()
{
  return mvvm::utils::GetStringList(anyvalueeditor::GetScalarTypeNames());
}
}  // namespace

namespace anyvalueeditor
{
AddFieldDialog::AddFieldDialog(QWidget* parent)
    : QDialog(parent)
    , m_name_field(new QLineEdit)
    , m_type_combo(new QComboBox)
    , m_scalar_type_combo(new QComboBox)
{
  // buttons
  auto button_box = new QDialogButtonBox;
  auto button = button_box->addButton("Add field", QDialogButtonBox::AcceptRole);
  button->setAutoDefault(false);
  button->setDefault(false);

  button = button_box->addButton("Cancel", QDialogButtonBox::RejectRole);
  button->setAutoDefault(false);
  button->setDefault(false);

  connect(button_box, &QDialogButtonBox::accepted, this, &AddFieldDialog::accept);
  connect(button_box, &QDialogButtonBox::rejected, this, &AddFieldDialog::reject);

  // combos setup
  m_type_combo->addItems({"scalar", "array", "struct"});
  m_type_combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  m_scalar_type_combo->addItems(CreateListOfScalars());
  m_scalar_type_combo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

  // grid
  auto grid = new QGridLayout;
  grid->addWidget(new QLabel("Name"), 0, 0, Qt::AlignLeft);
  grid->addWidget(m_name_field, 0, 1);
  grid->addWidget(new QLabel("Type"), 1, 0, Qt::AlignLeft);
  grid->addWidget(m_type_combo, 1, 1);
  grid->addWidget(new QLabel("ScalarType"), 2, 0, Qt::AlignLeft);
  grid->addWidget(m_scalar_type_combo, 2, 1);
  grid->setColumnStretch(0, 1);
  grid->setColumnStretch(1, 5);

  auto layout = new QVBoxLayout(this);
  layout->addLayout(grid);
  layout->addStretch(1);
  layout->addWidget(button_box);
}

FieldContext AddFieldDialog::GetFieldContext() const
{
  return {m_name_field->text().toStdString(), m_type_combo->currentText().toStdString(),
          m_scalar_type_combo->currentText().toStdString()};
}
}  // namespace anyvalueeditor
