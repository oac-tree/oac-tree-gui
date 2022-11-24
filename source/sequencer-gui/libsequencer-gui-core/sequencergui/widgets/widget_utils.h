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

#ifndef SEQUENCERGUI_WIDGETS_WIDGET_UTILS_H_
#define SEQUENCERGUI_WIDGETS_WIDGET_UTILS_H_

//! @file widgetutils.h
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
bool InvokeAndCatch(T method, const QString& description = {"Exception was caught"})
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
    msg_box.setText(description);
    msg_box.setDetailedText("Loren ipsum");
    msg_box.setInformativeText(QString(ex.what()));
    msg_box.setIcon(msg_box.Critical);
    msg_box.exec();
    return false;
  }
}

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_WIDGET_UTILS_H_
