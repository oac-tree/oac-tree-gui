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

#include "mockmodellistener.h"

#include "mvvm/model/sessionitem.h"

MockModelListener::MockModelListener(mvvm::ApplicationModel *model)  : ModelListener(model)
{
  auto on_about_to_insert = [this](auto item, auto tagindex)
  { OnAboutToInsertItem(item, tagindex); };
  SetOnAboutToInsertItem(on_about_to_insert);

  auto on_item_inserted = [this](auto item, auto tagindex) { OnItemInserted(item, tagindex); };
  SetOnItemInserted(on_item_inserted);

  auto on_about_to_remove = [this](auto item, auto tagindex)
  { OnAboutToRemoveItem(item, tagindex); };
  SetOnAboutToRemoveItem(on_about_to_remove);

  auto on_item_removed = [this](auto item, auto tagindex) { OnItemRemoved(item, tagindex); };
  SetOnItemRemoved(on_item_removed);

  auto on_data_changed = [this](auto item, auto role) { OnDataChanged(item, role); };
  SetOnDataChanged(on_data_changed);

  auto on_model_about_reset = [this](auto model) { OnModelAboutToBeReset(model); };
  SetOnModelAboutToBeReset(on_model_about_reset);

  auto on_model_reset = [this](auto model) { OnModelReset(model); };
  SetOnModelReset(on_model_reset);

  auto on_model_about_destroyed = [this](auto model) { OnModelAboutToBeDestroyed(model); };
  SetOnModelAboutToBeDestroyed(on_model_about_destroyed);
}
