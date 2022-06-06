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

#ifndef SEQUENCERGUI_MODEL_JOBMODEL_H
#define SEQUENCERGUI_MODEL_JOBMODEL_H

#include "mvvm/model/application_model.h"

namespace sequencergui
{

//! The model to hold running procedures.

class JobModel : public mvvm::ApplicationModel
{
public:
  JobModel();
  explicit JobModel(std::unique_ptr<mvvm::ItemManagerInterface> manager);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_JOBMODEL_H
