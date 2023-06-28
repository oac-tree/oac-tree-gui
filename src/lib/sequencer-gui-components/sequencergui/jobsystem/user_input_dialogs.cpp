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

#include "user_input_dialogs.h"

#include <sequencergui/domain/domain_utils.h>

#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

namespace sequencergui
{

UserChoiceResult GetUserChoiceDialogResult(const UserChoiceArgs &args)
{
  if (IsSelectTextDialog(args.metadata))
  {
    return GetTextSelectionDialogResult(args);
  }
  else if (IsMessageBoxDialog(args.metadata))
  {
    return GetConfirmationDialogResult(args);
  }

  return {0, false};
}

UserChoiceResult GetTextSelectionDialogResult(const UserChoiceArgs &args)
{
  QStringList selection_list;
  for (const auto &option : args.options)
  {
    selection_list.push_back(QString("%1").arg(QString::fromStdString(option)));
  }

  auto selection = QInputDialog::getItem(
      nullptr, "Input request", QString::fromStdString(GetMainTextFromMetadata(args.metadata)),
      selection_list);

  return {selection_list.indexOf(selection), true};
}

UserChoiceResult GetConfirmationDialogResult(const UserChoiceArgs &args)
{
  QMessageBox msg_box;
  msg_box.setWindowTitle(QString::fromStdString(GetTitleTextFromMetadata(args.metadata)));
  msg_box.setText(QString::fromStdString(GetMainTextFromMetadata(args.metadata)));
  msg_box.setIcon(QMessageBox::Information);

  auto option0_button = msg_box.addButton("OK", QMessageBox::AcceptRole);
  auto option1_button = msg_box.addButton("Cancel", QMessageBox::RejectRole);

  msg_box.exec();

  const int index = msg_box.clickedButton() == option0_button ? 0 : 1;

  return {index, true};
}

}  // namespace sequencergui
