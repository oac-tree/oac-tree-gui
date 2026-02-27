/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#include "oac_tree_gui/composer/instruction_editor_drop_handler.h"

#include <oac_tree_gui/core/exceptions.h>
#include <oac_tree_gui/viewmodel/instruction_editor_viewmodel.h>

#include <mvvm/model/application_model.h>

#include <gtest/gtest.h>

#include <QMimeData>

namespace oac_tree_gui::test
{

/**
 * @brief Testing InstructionEditorDropHandlerTest for basic editing scenarios.
 */
class InstructionEditorDropHandlerTest : public ::testing::Test
{
public:
  InstructionEditorDropHandlerTest() : m_view_model(&m_model) {}

  mvvm::ApplicationModel m_model;
  InstructionEditorViewModel m_view_model;
};

TEST_F(InstructionEditorDropHandlerTest, AttemptToCreateWithInitializedContext) {}

}  // namespace oac_tree_gui::test
