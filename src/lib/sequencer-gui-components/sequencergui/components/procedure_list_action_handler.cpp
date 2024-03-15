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

#include "procedure_list_action_handler.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/procedure_item.h>
#include <sequencergui/viewmodel/drag_and_drop_helper.h>

#include <mvvm/interfaces/sessionmodel_interface.h>
#include <mvvm/model/item_utils.h>
#include <mvvm/standarditems/container_item.h>

#include <QMimeData>

namespace sequencergui
{

ProcedureListActionHandler::ProcedureListActionHandler(ProcedureListContext context,
                                                       QObject *parent)
    : QObject(parent), m_context(std::move(context))
{
  if (!m_context.procedure_container || !m_context.selected_procedure)
  {
    throw RuntimeException("ProcedureList context is not fully initialised");
  }
}

void ProcedureListActionHandler::OnCreateNewProcedureRequest()
{
  auto selected = GetSelectedProcedure();
  auto tag_index = selected ? selected->GetTagIndex().Next() : mvvm::TagIndex::Append();
  auto procedure_item = GetModel()->InsertItem<ProcedureItem>(GetProcedureContainer(), tag_index);
  procedure_item->SetDisplayName("Untitled");
  // select just inserted procedure
  emit SelectProcedureRequest(procedure_item);
}

void ProcedureListActionHandler::OnRemoveProcedureRequest()
{
  if (auto selected_procedure = GetSelectedProcedure(); selected_procedure)
  {
    auto next_to_select = mvvm::utils::FindNextSiblingToSelect(selected_procedure);
    GetModel()->RemoveItem(selected_procedure);
    if (next_to_select)
    {
      // suggest to select something else instead of just deleted procedure
      emit SelectProcedureRequest(dynamic_cast<ProcedureItem *>(next_to_select));
    }
  }
}

bool ProcedureListActionHandler::CanCut() const
{
  return false;
}

void ProcedureListActionHandler::Cut() {}

bool ProcedureListActionHandler::CanCopy() const
{
  return GetSelectedProcedure() != nullptr;
}

void ProcedureListActionHandler::Copy()
{
  if (!m_context.set_mime_data)
  {
    throw RuntimeException("Context to copy mime data is not set");
  }

  if (auto selected = GetSelectedProcedure(); selected)
  {
    m_context.set_mime_data(CreateProcedureCopyMimeData(*selected));
  }
}

bool ProcedureListActionHandler::CanPaste() const
{
  if (auto mime_data = GetMimeData(); mime_data)
  {
    return mime_data->hasFormat(kCopyProcedureMimeType);
  }

  return false;
}

void ProcedureListActionHandler::Paste()
{
  auto mime_data = GetMimeData();
  if (!mime_data || !mime_data->hasFormat(kCopyProcedureMimeType))
  {
    return;
  }

  auto selected = GetSelectedProcedure();
  auto tag_index = selected ? selected->GetTagIndex().Next() : mvvm::TagIndex::Append();

  auto new_procedure = CreateProcedureItem(mime_data);
  auto new_procedure_ptr = new_procedure.get();

  GetModel()->InsertItem(std::move(new_procedure), GetProcedureContainer(), tag_index);

  // select just inserted procedure
  emit SelectProcedureRequest(new_procedure_ptr);
}

mvvm::ContainerItem *ProcedureListActionHandler::GetProcedureContainer() const
{
  return m_context.procedure_container();
}

ProcedureItem *ProcedureListActionHandler::GetSelectedProcedure() const
{
  return m_context.selected_procedure();
}

mvvm::SessionModelInterface *ProcedureListActionHandler::GetModel()
{
  return GetProcedureContainer()->GetModel();
}

const QMimeData *ProcedureListActionHandler::GetMimeData() const
{
  return m_context.get_mime_data ? m_context.get_mime_data() : nullptr;
}

}  // namespace sequencergui
