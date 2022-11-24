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

#ifndef SEQUENCERGUI_MAINWINDOW_ABOUT_APPLICATION_DIALOG_H_
#define SEQUENCERGUI_MAINWINDOW_ABOUT_APPLICATION_DIALOG_H_

#include <QDialog>

class QBoxLayout;

namespace sequencergui
{

//! About application dialog.

class AboutApplicationDialog : public QDialog
{
  Q_OBJECT
public:
  AboutApplicationDialog(QWidget* parent = 0);

private:
  QBoxLayout* createLogoLayout();
  QBoxLayout* createTextLayout();
  QBoxLayout* createButtonLayout();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MAINWINDOW_ABOUT_APPLICATION_DIALOG_H_
