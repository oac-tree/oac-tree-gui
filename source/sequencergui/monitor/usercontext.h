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

#ifndef MVVM_MONITOR_USERCONTEXT_H
#define MVVM_MONITOR_USERCONTEXT_H

#include <QString>
#include <functional>

namespace sequi
{

//! Aggregates necessary information to communicate with the user.
//!
struct UserContext
{
  //! Returns user input as a text. Initial vaue, and the description are provided as callback
  //! parameters.
  using userinput_callback_t = std::function<QString(QString, QString)>;

  userinput_callback_t m_user_input_callback;
};

}  // namespace sequi

#endif  // MVVM_MONITOR_USERCONTEXT_H
