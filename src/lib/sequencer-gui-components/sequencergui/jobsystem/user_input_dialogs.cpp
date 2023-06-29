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

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_utils.h>

#include <sup/sequencer/constants.h>

#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

namespace
{
QStringList GetSelectionList(const sequencergui::UserChoiceArgs &args)
{
  QStringList result;
  for (const auto &option : args.options)
  {
    result.push_back(QString("%1").arg(QString::fromStdString(option)));
  }
  return result;
}

}  // namespace

namespace sequencergui
{

UserChoiceResult GetUserChoiceDialogResult(const UserChoiceArgs &args)
{
  if (IsSelectTextDialog(args.metadata))
  {
    return GetTextSelectionDialogResult(args);
  }

  if (IsMessageBoxDialog(args.metadata))
  {
    return GetConfirmationDialogResult(args);
  }

  return {0, false};
}

UserChoiceResult GetTextSelectionDialogResult(const UserChoiceArgs &args)
{
  auto selection_list = GetSelectionList(args);

  const int selected_item_index{0};  // will be selected on combo box expand
  const bool is_editable{false};
  bool was_accepted{true};

  auto selection = QInputDialog::getItem(
      nullptr, "Input request", QString::fromStdString(GetMainTextFromMetadata(args.metadata)),
      selection_list, selected_item_index, is_editable, &was_accepted);

  // dialog's feature: it returns selected_item_index if cancel button was pressed

  return {was_accepted ? selection_list.indexOf(selection) : -1, was_accepted};
}

UserChoiceResult GetConfirmationDialogResult(const UserChoiceArgs &args)
{
  auto selection_list = GetSelectionList(args);
  if (selection_list.size() != 2)
  {
    throw RuntimeException("Can't make confirmation dialog out of given choice");
  }

  QMessageBox msg_box;
  msg_box.setWindowTitle(QString::fromStdString(GetTitleTextFromMetadata(args.metadata)));
  msg_box.setText(QString::fromStdString(GetMainTextFromMetadata(args.metadata)));
  msg_box.setIcon(QMessageBox::Information);

  auto option0_button = msg_box.addButton(selection_list.at(0), QMessageBox::AcceptRole);
  auto option1_button = msg_box.addButton(selection_list.at(1), QMessageBox::RejectRole);

  msg_box.exec();

  const int index = msg_box.clickedButton() == option0_button ? 0 : 1;

  return {index, true};
}

}  // namespace sequencergui
