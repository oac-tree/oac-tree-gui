/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "Instruction.h"
#include "sequencergui/model/sequencer_types.h"
#include "sequencergui/model/sequenceritems.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>
#include <QMetaType>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  QApplication app(argc, argv);
  Q_UNUSED(app)

  qRegisterMetaType<sequi::InstructionItem*>("sequi::InstructionItem*");
  qRegisterMetaType<const sequi::InstructionItem*>("const sequi::InstructionItem*");
  qRegisterMetaType<instruction_t*>("instruction_t*");
  qRegisterMetaType<const instruction_t*>("const instruction_t*");

  // run all google tests
  return RUN_ALL_TESTS();
}
