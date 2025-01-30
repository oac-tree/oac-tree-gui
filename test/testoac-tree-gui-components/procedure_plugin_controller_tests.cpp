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

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/domain/domain_helper.h>
#include <oac_tree_gui/model/epics_instruction_items.h>
#include <oac_tree_gui/model/instruction_container_item.h>
#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/procedure_preamble_items.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>
#include <oac_tree_gui/model/standard_variable_items.h>
#include <oac_tree_gui/model/workspace_item.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

#include "oac_tree_gui/composer/procedure_plugin_controller.h"

namespace oac_tree_gui::test
{

//! Tests of ProcedurePluginController class.

class ProcedurePluginControllerTest : public ::testing::Test
{
};

//! Validating how controller updates procedure preamble, when procedure contains only core
//! instructions and variables.
TEST_F(ProcedurePluginControllerTest, CorePluginScenario)
{
  SequencerModel model;
  ProcedurePluginController controller(&model);

  auto procedure0 =
      model.InsertItem<ProcedureItem>(model.GetProcedureContainer(), mvvm::TagIndex::Append());

  EXPECT_TRUE(procedure0->GetPreambleItem()->GetPluginPaths().empty());

  procedure0->GetWorkspace()->InsertItem<LocalVariableItem>(mvvm::TagIndex::Append());
  procedure0->GetInstructionContainer()->InsertItem<SequenceItem>(mvvm::TagIndex::Append());

  // No extra plugins since LocalVariable and Sequence are located in the main sequencer lib.
  EXPECT_TRUE(procedure0->GetPreambleItem()->GetPluginPaths().empty());
}

//! Validating how controller updates procedure preamble, when procedure contains EPICS core
//! instructions and variables.
TEST_F(ProcedurePluginControllerTest, EPICSPluginScenario)
{
  if (!IsSequencerPluginEpicsAvailable())
  {
    GTEST_SKIP();
  }

  SequencerModel model;
  ProcedurePluginController controller(&model);

  auto procedure0 =
      model.InsertItem<ProcedureItem>(model.GetProcedureContainer(), mvvm::TagIndex::Append());

  EXPECT_TRUE(procedure0->GetPreambleItem()->GetPluginPaths().empty());

  // adding ChannelAccessVariableItem, which requires a plugin
  model.InsertItem<ChannelAccessVariableItem>(procedure0->GetWorkspace(), mvvm::TagIndex::Append());
  EXPECT_EQ(procedure0->GetPreambleItem()->GetPluginPaths(),
            std::vector<std::string>({domainconstants::kEpicsCAPluginName}));

  // adding Sequence that doesn't need more plugins
  auto sequence = model.InsertItem<SequenceItem>(procedure0->GetInstructionContainer(),
                                                 mvvm::TagIndex::Append());
  EXPECT_EQ(procedure0->GetPreambleItem()->GetPluginPaths(),
            std::vector<std::string>({domainconstants::kEpicsCAPluginName}));

  // adding PvAccessWriteInstructionItem inside Sequence
  model.InsertItem<PvAccessWriteInstructionItem>(sequence, mvvm::TagIndex::Append());

  EXPECT_EQ(procedure0->GetPreambleItem()->GetPluginPaths(),
            std::vector<std::string>(
                {domainconstants::kEpicsCAPluginName, domainconstants::kEpicsPVXSPluginName}));
}

}  // namespace oac_tree_gui::test
