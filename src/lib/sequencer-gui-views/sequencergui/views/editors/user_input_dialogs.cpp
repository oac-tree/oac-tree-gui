/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#include "anyvalue_editor_dialog_factory.h"

#include <sequencergui/components/anyvalue_dialog_result.h>
#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_helper.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/views/anyvalueeditor/anyvalue_editor_dialog.h>

#include <mvvm/widgets/widget_utils.h>

#include <sup/sequencer/constants.h>

#include <QInputDialog>
#include <QMainWindow>
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

UserChoiceResult GetUserChoiceDialogResult(const UserChoiceArgs &args, QWidget *parent)
{
  if (IsSelectTextDialog(args.metadata))
  {
    return GetTextSelectionDialogResult(args, parent);
  }

  if (IsMessageBoxDialog(args.metadata))
  {
    return GetConfirmationDialogResult(args, parent);
  }

  return {0, false};
}

UserChoiceResult GetTextSelectionDialogResult(const UserChoiceArgs &args, QWidget *parent)
{
  auto selection_list = GetSelectionList(args);

  const int selected_item_index{0};  // will be selected on combo box expand
  const bool is_editable{false};
  bool was_accepted{true};

  QString selection = QInputDialog::getItem(
      parent, "Input request", QString::fromStdString(GetMainTextFromMetadata(args.metadata)),
      selection_list, selected_item_index, is_editable, &was_accepted);

  // dialog's feature: it returns selected_item_index if cancel button was pressed

  auto index = static_cast<int>(selection_list.indexOf(selection));
  return {was_accepted ? index : -1, was_accepted};
}

UserChoiceResult GetConfirmationDialogResult(const UserChoiceArgs &args, QWidget *parent)
{
  auto selection_list = GetSelectionList(args);
  if (selection_list.size() != 2)
  {
    throw RuntimeException("Can't make confirmation dialog out of given choice");
  }

  QMessageBox msg_box(parent);
  msg_box.setWindowTitle(QString::fromStdString(GetTitleTextFromMetadata(args.metadata)));
  msg_box.setText(QString::fromStdString(GetMainTextFromMetadata(args.metadata)));
  msg_box.setIcon(QMessageBox::Information);

  auto option0_button = msg_box.addButton(selection_list.at(0), QMessageBox::AcceptRole);
  auto option1_button = msg_box.addButton(selection_list.at(1), QMessageBox::RejectRole);
  (void) option1_button;

  msg_box.exec();

  const int index = msg_box.clickedButton() == option0_button ? 0 : 1;

  return {index, true};
}

UserInputResult GetAnyValueEditorDialogResult(const UserInputArgs &args, QWidget *parent)
{
  auto parent_widget = parent ? parent : mvvm::utils::FindMainWindow();

  auto anyvalue_item = sup::gui::CreateItem(args.value);
  std::unique_ptr<sup::gui::AnyValueEditorDialog> dialog;
  if (anyvalue_item->IsScalar())
  {
    dialog = CreateAnyValueCompactScalarEditorDialog(anyvalue_item.get(), parent_widget);
  }
  else
  {
    dialog = CreateAnyValueCompactTreeEditorDialog(anyvalue_item.get(), parent_widget);
  }
  dialog->SetDescription(QString::fromStdString(args.description));
  if (dialog->exec() == QDialog::Accepted)
  {
    auto anyvalue = sup::gui::CreateAnyValue(*dialog->GetResult());
    return UserInputResult{anyvalue, true};
  }

  return UserInputResult{{}, false};
}

UserContext CreateDefaultUserContext(QWidget *parent)
{
  UserContext result;
  result.user_choice_callback = [parent](const auto &args)
  { return GetUserChoiceDialogResult(args, parent); };

  result.user_input_callback = [parent](const auto &args)
  { return GetAnyValueEditorDialogResult(args, parent); };

  return result;
}

}  // namespace sequencergui
