/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "remote_connection_dialog.h"

#include <sup/gui/widgets/dialog_helper.h>

#include <QVBoxLayout>

namespace sequencergui
{

RemoteConnectionDialog::RemoteConnectionDialog(QWidget *parent) : QDialog(parent)
{
  auto layout = new QVBoxLayout(this);
  layout->addWidget(new QWidget);
  layout->addLayout(sup::gui::CreateButtonLayout(this).release());
}

RemoteConnectionDialog::~RemoteConnectionDialog() = default;

}  // namespace sequencergui
