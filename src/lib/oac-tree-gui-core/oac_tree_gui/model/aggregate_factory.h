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

#ifndef OAC_TREE_GUI_MODEL_AGGREGATE_FACTORY_H_
#define OAC_TREE_GUI_MODEL_AGGREGATE_FACTORY_H_

#include <oac_tree_gui/core/factory.h>

#include <functional>
#include <memory>

namespace oac_tree_gui
{
class InstructionItem;

//! Allows to build examples representing complex instruction aggregates.

class AggregateFactory
    : public Factory<std::string, std::function<std::unique_ptr<InstructionItem>()>>
{
public:
  AggregateFactory();

  std::unique_ptr<InstructionItem> Create(const std::string& type_name);
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_AGGREGATE_FACTORY_H_
