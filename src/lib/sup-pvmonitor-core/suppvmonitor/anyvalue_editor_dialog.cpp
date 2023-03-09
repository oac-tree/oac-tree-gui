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

#include "anyvalue_editor_dialog.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace suppvmonitor
{

AnyValueEditorDialog::AnyValueEditorDialog(QWidget* parent)
    : QDialog(parent)
{
  // buttons
  auto button_box = new QDialogButtonBox;
  auto button = button_box->addButton("Add field", QDialogButtonBox::AcceptRole);
  button->setAutoDefault(false);
  button->setDefault(false);

  button = button_box->addButton("Cancel", QDialogButtonBox::RejectRole);
  button->setAutoDefault(false);
  button->setDefault(false);

  connect(button_box, &QDialogButtonBox::accepted, this, &AnyValueEditorDialog::accept);
  connect(button_box, &QDialogButtonBox::rejected, this, &AnyValueEditorDialog::reject);

  auto layout = new QVBoxLayout(this);
}

}  // namespace anyvalueeditor
