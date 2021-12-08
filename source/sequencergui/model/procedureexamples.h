/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef MVVM_MODEL_PROCEDUREEXAMPLES_H
#define MVVM_MODEL_PROCEDUREEXAMPLES_H

//! @file procedureexamples.h
//! Collection of example procedures to populate the model for debugging purposes.

#include <memory>

namespace sequi
{
class SequencerModel;
class ProcedureItem;

namespace Examples
{

//! Variable copying example.
ProcedureItem*  AddCopyProcedure(SequencerModel* model);

//! Local include example.
ProcedureItem*  AddLocalIncludeProcedure(SequencerModel* model);

//! User input example.
ProcedureItem* AddInputProcedure(SequencerModel* model);

}
}  // namespace sequi

#endif  // MVVM_MODEL_PROCEDUREEXAMPLES_H
