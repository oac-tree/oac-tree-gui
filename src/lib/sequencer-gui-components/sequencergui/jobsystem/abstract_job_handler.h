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

#ifndef SEQUENCERGUI_JOBSYSTEM_ABSTRACT_JOB_HANDLER_H_
#define SEQUENCERGUI_JOBSYSTEM_ABSTRACT_JOB_HANDLER_H_

namespace sequencergui
{

/**
 * @brief The AbstractJobHandler is a base class to run a jobs represented by the JobItem.
 */
class AbstractJobHandler
{
public:
  virtual ~AbstractJobHandler();

};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_ABSTRACT_JOB_HANDLER_H_
