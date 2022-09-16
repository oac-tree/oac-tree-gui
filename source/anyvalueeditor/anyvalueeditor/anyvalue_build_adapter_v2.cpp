/******************************************************************************
 *
 * Project       : Supervision and automation system EPICS interface
 *
 * Description   : Library of SUP components for EPICS network protocol
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

#include "anyvalueeditor/anyvalue_build_adapter_v2.h"

#include <sup/dto/anytype.h>
#include <sup/dto/anyvalue.h>

#include <stack>
#include <stdexcept>

namespace anyvalueeditor
{

struct AnyValueBuildAdapterV2::AnyValueBuildAdapterV2Impl
{
};

AnyValueBuildAdapterV2::AnyValueBuildAdapterV2() : p_impl(new AnyValueBuildAdapterV2Impl) {}

AnyValueBuildAdapterV2::~AnyValueBuildAdapterV2() = default;

}  // namespace anyvalueeditor
