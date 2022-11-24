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

#include <sup/sequencer/instruction.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/job_item.h>
#include <sequencergui/model/sequencer_types.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>
#include <QMetaType>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  qRegisterMetaType<sequencergui::InstructionItem*>("sequencergui::InstructionItem*");
  qRegisterMetaType<const sequencergui::InstructionItem*>("const sequencergui::InstructionItem*");
  qRegisterMetaType<instruction_t*>("instruction_t*");
  qRegisterMetaType<const instruction_t*>("const instruction_t*");
  qRegisterMetaType<mvvm::SessionItem*>("mvvm::SessionItem*");
  qRegisterMetaType<sequencergui::JobItem*>("sequencergui::JobItem*");

  QApplication app(argc, argv);
  Q_UNUSED(app)

  // run all google tests
  return RUN_ALL_TESTS();
}
