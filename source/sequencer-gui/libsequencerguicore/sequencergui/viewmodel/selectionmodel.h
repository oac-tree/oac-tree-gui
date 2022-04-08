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

#ifndef SEQUENCERGUI_VIEWMODEL_SELECTIONMODEL_H
#define SEQUENCERGUI_VIEWMODEL_SELECTIONMODEL_H

#include <QItemSelectionModel>

namespace mvvm
{
class ViewModel;
}

namespace sequencergui
{

class SelectionModel : public QItemSelectionModel
{
  Q_OBJECT

public:
  explicit SelectionModel(mvvm::ViewModel* view_model, QObject* parent = nullptr);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWMODEL_SELECTIONMODEL_H
