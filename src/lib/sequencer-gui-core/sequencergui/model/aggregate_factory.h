/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_AGGREGATE_FACTORY_H_
#define SEQUENCERGUI_MODEL_AGGREGATE_FACTORY_H_

#include <sequencergui/core/factory.h>

#include <functional>
#include <memory>

namespace sequencergui
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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_AGGREGATE_FACTORY_H_
