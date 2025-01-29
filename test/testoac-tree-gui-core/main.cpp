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

#include <oac-tree-gui/domain/domain_helper.h>
#include <oac-tree-gui/domain/sequencer_types_fwd.h>
#include <oac-tree-gui/model/instruction_item.h>
#include <oac-tree-gui/model/register_items.h>

#include <sup/oac-tree/instruction.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QMetaType>

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);

  sequencergui::RegisterSessionItems();

  qRegisterMetaType<sequencergui::InstructionItem*>("sequencergui::InstructionItem*");
  qRegisterMetaType<const sequencergui::InstructionItem*>("const sequencergui::InstructionItem*");
  qRegisterMetaType<instruction_t*>("instruction_t*");
  qRegisterMetaType<const instruction_t*>("const instruction_t*");

  sequencergui::LoadPlugins();

  // run all google tests
  return RUN_ALL_TESTS();
}
