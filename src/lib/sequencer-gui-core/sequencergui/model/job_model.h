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

#ifndef SEQUENCERGUI_MODEL_JOB_MODEL_H_
#define SEQUENCERGUI_MODEL_JOB_MODEL_H_

#include <mvvm/model/application_model.h>

namespace sequencergui
{

//! The model to hold running procedures.

class JobModel : public mvvm::ApplicationModel
{
public:
  explicit JobModel(std::shared_ptr<mvvm::ItemPool> pool = {});
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_JOB_MODEL_H_
