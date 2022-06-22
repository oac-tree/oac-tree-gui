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

#ifndef SEQUENCERGUI_MODEL_JOBITEM_H
#define SEQUENCERGUI_MODEL_JOBITEM_H

#include <mvvm/model/compound_item.h>

namespace sequencergui
{

class ProcedureItem;

//! Represents job.
class JobItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "Job";
  JobItem();

  std::string GetStatus() const;

  void SetStatus(const std::string& status);

  void SetProcedure(const ProcedureItem* item);

  ProcedureItem* GetProcedure() const;

  ProcedureItem* GetExpandedProcedure();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_JOBITEM_H
