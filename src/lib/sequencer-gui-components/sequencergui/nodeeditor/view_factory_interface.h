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

#ifndef SEQUENCERGUI_NODEEDITOR_VIEW_FACTORY_INTERFACE_H_
#define SEQUENCERGUI_NODEEDITOR_VIEW_FACTORY_INTERFACE_H_

#include <memory>

namespace sequencergui
{

class InstructionItem;
class ConnectableView;

class ViewFactoryInterface
{
public:
  virtual ~ViewFactoryInterface() = default;
  ViewFactoryInterface() = default;
  ViewFactoryInterface(const ViewFactoryInterface&) = delete;
  ViewFactoryInterface& operator=(const ViewFactoryInterface&) = delete;

  virtual std::unique_ptr<ConnectableView> CreateView(InstructionItem*) = 0;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_VIEW_FACTORY_INTERFACE_H_
