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

#include "sequencergui/composer/attribute_editor_action_handler.h"

#include <sup/gui/model/anyvalue_item.h>

#include <gtest/gtest.h>
#include <testutils/test_utils.h>

using namespace sequencergui;

Q_DECLARE_METATYPE(mvvm::SessionItem*)

//! Tests for AttributeEditorActionHandler class.

class AttributeEditorActionHandlerTest : public ::testing::Test
{
};

TEST_F(AttributeEditorActionHandlerTest, InitialState) {}
