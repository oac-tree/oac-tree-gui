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

#include <sequencergui/core/version.h>
#include <sequencergui/mainwindow/main_window_helper.h>
#include <sequencergui/mainwindow/sequencer_main_window.h>

int main(int argc, char** argv)
{
  auto version = QString::fromStdString(sequencergui::ProjectVersion());
  sup::gui::InitCoreApplication("sequencer-gui", version);

  return sequencergui::RunApplication<sequencergui::SequencerMainWindow>(argc, argv);
}
