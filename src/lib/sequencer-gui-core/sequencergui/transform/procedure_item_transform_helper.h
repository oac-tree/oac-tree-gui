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

#ifndef SEQUENCERGUI_TRANSFORM_PROCEDURE_ITEM_TRANSFORM_HELPER_H_
#define SEQUENCERGUI_TRANSFORM_PROCEDURE_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of utility functions to convert sup::sequencer::Procedure into SessionModel.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <memory>

namespace sequencergui
{

class ProcedureItem;
class ProcedurePreambleItem;

/**
 * @brief Populates empty PreambleItem with the domain information.
 */
void PopulateProcedurePreambleItem(const preamble_t& preamble, ProcedurePreambleItem& item);

/**
 * @brief Creates ProcedureItem from given domain procedure.
 *
 * Its main purpose is to generate a GUI object from the result of XML import. It is expected, that
 * the domain procedure's Setup() hasn't been performed yet.
 */
std::unique_ptr<ProcedureItem> CreateProcedureItem(const procedure_t& procedure);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_PROCEDURE_ITEM_TRANSFORM_HELPER_H_
