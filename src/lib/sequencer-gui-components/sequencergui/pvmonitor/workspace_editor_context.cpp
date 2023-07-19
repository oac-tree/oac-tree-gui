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

#include "workspace_editor_context.h"

#include "anyvalue_editor_dialog.h"

#include <sup/gui/model/anyvalue_item.h>

namespace sequencergui
{

std::function<AnyValueDialogResult(const sup::gui::AnyValueItem *)> CreateAnyValueDialogCallback()
{
  auto edit_anyvalue_callback = [](const sup::gui::AnyValueItem *item) -> AnyValueDialogResult
  {
    AnyValueEditorDialog dialog;
    dialog.SetInitialValue(item);
    if (dialog.exec() == QDialog::Accepted)
    {
      return {true, dialog.GetResult()};
    }
    return {false, {}};  // returning false denotes that dialog was canceled
  };

  return edit_anyvalue_callback;
}

}  // namespace sequencergui
