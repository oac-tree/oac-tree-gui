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

#ifndef LIBTEST_UTILS_TESTUTILS_TEST_NODE_EDITOR_HELPER_H_
#define LIBTEST_UTILS_TESTUTILS_TEST_NODE_EDITOR_HELPER_H_

//! @file
//! Collection of utility functions to test node editor.

#include <vector>

#include <QPointF>

class QGraphicsScene;

namespace oac_tree_gui
{
class InstructionItem;
}

namespace mvvm
{
class ConnectableShape;
}

namespace oac_tree_gui::test
{

/**
 * @brief Finds all underlying instructions by exploring shapes on the scene.
 */
std::vector<InstructionItem*> FindSceneInstructions(const QGraphicsScene& scene);

/**
 * @brief Returns scene position of input port.
 *
 * Will throw if input port is not found.
 */
QPointF GetInputPortScenePosition(const mvvm::ConnectableShape& shape);


/**
 * @brief Returns scene position of output port.
 *
 * Will throw if output port is not found.
 */
QPointF GetOutputPortScenePosition(const mvvm::ConnectableShape& shape);

}  // namespace oac_tree_gui::test

#endif  // LIBTEST_UTILS_TESTUTILS_TEST_NODE_EDITOR_HELPER_H_
