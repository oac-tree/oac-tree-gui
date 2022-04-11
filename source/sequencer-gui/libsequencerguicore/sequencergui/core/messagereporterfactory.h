/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_CORE_MESSAGEREPORTERFACTORY_H
#define SEQUENCERGUI_CORE_MESSAGEREPORTERFACTORY_H

//! @file messagereporterfactory.h
//! Factory functions to create various message reporters.

#include "sequencergui/core/messageinterface.h"

#include <memory>
namespace sequencergui
{

//! Creates
std::unique_ptr<MessageInterface> CreateStdMessageHandler();

}

#endif // SEQUENCERGUI_CORE_MESSAGEREPORTERFACTORY_H
