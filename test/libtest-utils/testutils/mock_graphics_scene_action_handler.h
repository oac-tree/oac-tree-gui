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

#ifndef LIBTEST_UTILS_TESTUTILS_MOCK_GRAPHICS_SCENE_ACTION_HANDLER_H_
#define LIBTEST_UTILS_TESTUTILS_MOCK_GRAPHICS_SCENE_ACTION_HANDLER_H_

#include <oac_tree_gui/nodeeditor/i_graphics_scene_action_handler.h>

#include <gmock/gmock.h>

namespace oac_tree_gui::test
{

/**
 * @brief The MockGraphicsSceneActionHandler is intended to test calls to
 * IGraphicsSceneActionHandler interface.
 */
class MockGraphicsSceneActionHandler : public IGraphicsSceneActionHandler
{
public:
  MOCK_METHOD(void, ConnectPorts,
              (const mvvm::INodePort* start_port, const mvvm::INodePort* end_port), (override));
  MOCK_METHOD(void, Disconnect, (mvvm::INodeConnection * connection), (override));
};

}  // namespace oac_tree_gui::test

#endif
