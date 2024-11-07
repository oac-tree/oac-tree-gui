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

#include <string>

namespace sequencergui
{

/**
 * @brief The IAutomationClient class holds automation server connection information.
 *
 * It is used to create remote jobs.
 * Internally holds sup::sequencer::IJobManager.
 */
class IAutomationClient
{
public:
  virtual ~IAutomationClient() = default;

  virtual std::string GetServerName() const = 0;
};

}  // namespace sequencergui
