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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "oac_tree_gui/composer/objects/procedure_editor.h"

#include <oac_tree_gui/model/instruction_container_item.h>
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
  InstructionContainerItem* GetInstructionContainer() const
  {
    return m_procedure->GetInstructionContainer();
  }

  SequencerModel m_model;
  ProcedureItem* m_procedure{nullptr};
};

TEST_F(ProcedureEditorTest, AttemptToCreateWhenNoContextIsInitialised)
{
  ProcedureEditor editor({});

  EXPECT_TRUE(editor.GetInstructionInFocus().empty());
  SequenceItem item;
  const std::vector<InstructionItem*> instructions({&item});
  editor.SetInstructionsInFocus(instructions);
  EXPECT_EQ(editor.GetInstructionInFocus(), instructions);
}

TEST_F(ProcedureEditorTest, InsertInstructionFromToolBox)
{
  ProcedureEditor editor({});

  editor.SetProcedure(m_procedure);

  editor.InsertInstructionFromToolBox(QString::fromStdString(SequenceItem::GetStaticType()));
  EXPECT_EQ(GetInstructionContainer()->GetInstructionCount(), 1);
}

}  // namespace oac_tree_gui::test
