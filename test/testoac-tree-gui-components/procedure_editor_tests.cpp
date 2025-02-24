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

#include "oac_tree_gui/composer/procedure_editor.h"

#include <oac_tree_gui/model/procedure_item.h>
#include <oac_tree_gui/model/sequencer_model.h>
#include <oac_tree_gui/model/standard_instruction_items.h>

#include <mvvm/standarditems/container_item.h>

#include <gtest/gtest.h>

namespace oac_tree_gui::test
{

/**
 * @brief Testing ProcedureEditor.
 */
class ProcedureEditorTest : public ::testing::Test
{
public:
  ProcedureEditorTest()
  {
    m_procedure = m_model.InsertItem<ProcedureItem>(m_model.GetProcedureContainer());
  }

  SequencerModel m_model;
  ProcedureItem* m_procedure{nullptr};
};

TEST_F(ProcedureEditorTest, AttemptToCreateWhenNoContextIsInitialised)
{
  ProcedureEditor editor;

  EXPECT_TRUE(editor.GetInstructionInFocus().empty());
  SequenceItem item;
  std::vector<InstructionItem*> instructions({&item});
  editor.SetInstructionsInFocus(instructions);
  EXPECT_EQ(editor.GetInstructionInFocus(), instructions);
}

}  // namespace oac_tree_gui::test
