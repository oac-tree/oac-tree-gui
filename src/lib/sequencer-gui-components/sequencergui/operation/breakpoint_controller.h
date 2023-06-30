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

#ifndef SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_
#define SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_

#include <QObject>

namespace sequencergui
{

/**
 * @brief The BreakpointController class stores information about breakpoints, and controls its
 * propagation to the domain runner.
 */

class BreakpointController : public QObject
{
  Q_OBJECT

public:
  explicit BreakpointController(QObject* parent = nullptr);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_BREAKPOINT_CONTROLLER_H_
