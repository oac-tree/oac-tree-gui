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

#ifndef SEQUENCERGUI_WIDGETS_WIDGET_UTILS_H_
#define SEQUENCERGUI_WIDGETS_WIDGET_UTILS_H_

//! Collection of general widget-related utility functions.

#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <functional>

namespace sequencergui
{

//! Safe invoke of user provided method.
//! Provides busy-sign while executing, and warning dialog on exception catch.

template <typename T>
bool InvokeAndCatch(T method, const QString& text = {"Exception was caught"},
                    const QString& informative_text = {})
{
  try
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    std::invoke(method);
    QApplication::restoreOverrideCursor();
    return true;
  }
  catch (const std::exception& ex)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox msg_box;
    msg_box.setText(text);

    QString exception_message(ex.what());

    if (informative_text.isEmpty())
    {
      msg_box.setInformativeText(exception_message);
    }
    else
    {
      msg_box.setInformativeText(informative_text);
      msg_box.setDetailedText(exception_message);
    }

    msg_box.setIcon(msg_box.Critical);
    msg_box.exec();
    return false;
  }
}

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_WIDGET_UTILS_H_
