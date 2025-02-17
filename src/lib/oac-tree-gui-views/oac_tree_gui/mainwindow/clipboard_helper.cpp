/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
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

#include "clipboard_helper.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>

namespace oac_tree_gui
{

get_mime_data_func_t DefaultClipboardGetFunc()
{
  return []() { return QGuiApplication::clipboard()->mimeData(); };
}

set_mime_data_func_t DefaultClipboardSetFunc()
{
  return [](std::unique_ptr<QMimeData> data)
  { QGuiApplication::clipboard()->setMimeData(data.release()); };
}

}  // namespace oac_tree_gui
