/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_VIEWFACTORYINTERFACE_H
#define SEQUENCERGUI_NODEEDITOR_VIEWFACTORYINTERFACE_H

#include <memory>

namespace sequencergui
{

class InstructionItem;
class ConnectableView;

class ViewFactoryInterface
{
public:
  virtual std::unique_ptr<ConnectableView> CreateView(InstructionItem*) = 0;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_VIEWFACTORYINTERFACE_H
