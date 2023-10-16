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

#ifndef SEQUENCERGUI_VIEWMODEL_CUSTOM_PRESENTATION_ITEMS_H
#define SEQUENCERGUI_VIEWMODEL_CUSTOM_PRESENTATION_ITEMS_H

#include <mvvm/viewmodel/standard_presentation_items.h>

namespace sequencergui
{

//! Presentation for IsAvailable variable status in the form of colored box.

class MVVM_VIEWMODEL_EXPORT AvailablePresentationItem : public mvvm::DataPresentationItem
{
public:
  explicit AvailablePresentationItem(mvvm::SessionItem* item);

  QVariant Data(int qt_role) const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_CUSTOM_PRESENTATION_ITEMS_H
